import { GL, rawScenes } from "../app/App"
import { getCurrScene } from "../redux/store"

import Model from "../sceneGraph/Model"
import Camera from "../sceneGraph/Camera"
import Vector1x4 from "../math/Vector1x4"
import OmniDirLS from "../sceneGraph/OmniDirLS"
import { Material, ModelPiece } from "../types/scene"

const g_up = new Vector1x4(0.0, 0.0, 1.0, 0.0)
const g_origin = new Vector1x4(0.0, 0.0, 0.0, 1.0)

export interface VertexAttributeDesc {
  attrib: string
  length: number
  stride: number
  offset: number
}

async function fetchText(url: string): Promise<string> {
  const response = await fetch(url)
  if (!response.ok) {
    throw new Error(`Cannot GET ${url} status=${response.status}`)
  }
  return response.text()
}

export default class Shader {
  program: WebGLProgram | null
  edgeBuffer: WebGLBuffer

  constructor(vertShaderURL: string, fragShaderURL: string) {
    if (!GL) {
      throw new Error("WebGL context not initialized")
    }
    const gl = GL

    this.program = null
    this.edgeBuffer = gl.createBuffer() as WebGLBuffer

    void (async () => {
      const [vertSource, fragSource] = await Promise.all([fetchText(vertShaderURL), fetchText(fragShaderURL)])

      const vs = gl.createShader(gl.VERTEX_SHADER) as WebGLShader
      const fs = gl.createShader(gl.FRAGMENT_SHADER) as WebGLShader
      gl.shaderSource(vs, vertSource)
      gl.shaderSource(fs, fragSource)
      gl.compileShader(vs)
      gl.compileShader(fs)

      if (!gl.getShaderParameter(vs, gl.COMPILE_STATUS)) {
        throw new Error("Error compiling " + vertShaderURL + " !\n" + gl.getShaderInfoLog(vs))
      }
      if (!gl.getShaderParameter(fs, gl.COMPILE_STATUS)) {
        throw new Error("Error compiling " + fragShaderURL + " !\n" + gl.getShaderInfoLog(fs))
      }

      const program = gl.createProgram()
      if (!program) {
        throw new Error("Error creating shader program!")
      }
      this.program = program
      gl.attachShader(program, vs)
      gl.attachShader(program, fs)
      gl.linkProgram(program)

      if (!gl.getProgramParameter(program, gl.LINK_STATUS)) {
        throw new Error("Error linking shader program!\n")
      }
    })()
  }

  drawTriangles(model: Model, modelPiece: ModelPiece) {
    if (this.program && GL) {
      const { material, vtxBuffer, idxBuffer } = modelPiece

      GL.useProgram(this.program)
      this.setUniformVariablesInVertShader(model)
      this.setUniformVariablesInFragShader(model, material)
      GL.bindBuffer(GL.ARRAY_BUFFER, vtxBuffer)
      GL.bindBuffer(GL.ELEMENT_ARRAY_BUFFER, idxBuffer)

      for (const desc of (this.constructor as typeof Shader).vertexAttributeDescs) {
        const loc = GL.getAttribLocation(this.program, desc.attrib)
        if (loc !== -1) {
          GL.vertexAttribPointer(loc, desc.length, GL.FLOAT, false, desc.stride, desc.offset)
          GL.enableVertexAttribArray(loc)
        }
      }

      GL.drawElements(GL.TRIANGLES, modelPiece.triVtxCount, GL.UNSIGNED_SHORT, 0)
    }
  }

  drawTriangleEdges(model: Model, modelPiece: ModelPiece, cullBackFacing = true) {
    const currScene = getCurrScene()
    if (currScene && GL && this.program) {
      const camPosition = rawScenes[currScene.id].activeCamera.mapPos(g_origin, model)
      const edgeIndices: number[] = []

      const idxs = modelPiece.idxs
      const vtxs = modelPiece.vtxs

      for (let i = 0; i < idxs.length; i += 3) {
        let j = idxs[i] * (this.constructor as typeof Shader).vertexElementCount
        const vx = camPosition.x - vtxs[j++]
        const vy = camPosition.y - vtxs[j++]
        const vz = camPosition.z - vtxs[j++]
        const nx = vtxs[j++]
        const ny = vtxs[j++]
        const nz = vtxs[j]

        if (!cullBackFacing || vx * nx + vy * ny + vz * nz > 0) {
          edgeIndices.push(idxs[i])
          edgeIndices.push(idxs[i + 1])
          edgeIndices.push(idxs[i + 1])
          edgeIndices.push(idxs[i + 2])
          edgeIndices.push(idxs[i + 2])
          edgeIndices.push(idxs[i])
        }
      }

      GL.useProgram(this.program)
      this.setUniformVariablesInVertShader(model)
      this.setUniformVariablesInFragShader(model, modelPiece.material)
      GL.bindBuffer(GL.ARRAY_BUFFER, modelPiece.vtxBuffer)
      GL.bindBuffer(GL.ELEMENT_ARRAY_BUFFER, this.edgeBuffer)
      GL.bufferData(GL.ELEMENT_ARRAY_BUFFER, new Uint16Array(edgeIndices), GL.STATIC_DRAW)

      for (const desc of (this.constructor as typeof Shader).vertexAttributeDescs) {
        const loc = GL.getAttribLocation(this.program, desc.attrib)
        if (loc !== -1) {
          GL.vertexAttribPointer(loc, desc.length, GL.FLOAT, false, desc.stride, desc.offset)
          GL.enableVertexAttribArray(loc)
        }
      }

      GL.drawElements(GL.LINES, edgeIndices.length, GL.UNSIGNED_SHORT, 0)
    }
  }

  setUniformVariablesInVertShader(model: Model) {
    const currScene = getCurrScene()
    if (currScene && GL) {
      const scene = rawScenes[currScene.id]
      const omniDirLS = scene.omniDirLS
      const activeCam = scene.activeCamera

      const loc0 = GL.getUniformLocation(this.program!, "u_attnCoeffs")
      if (loc0 && omniDirLS instanceof OmniDirLS) {
        GL.uniform3f(loc0, omniDirLS.coeff0, omniDirLS.coeff1, omniDirLS.coeff2)
      }

      const loc1 = GL.getUniformLocation(this.program!, "u_model_view_proj_matrix")
      if (loc1) {
        const modelViewProjMatrix = model.modelMatrix.mul(activeCam.viewProjMatrix)
        GL.uniformMatrix4fv(loc1, false, modelViewProjMatrix.toFloat32Array())
      }

      const loc2 = GL.getUniformLocation(this.program!, "u_camera_pos")
      if (loc2 && activeCam instanceof Camera && model instanceof Model) {
        const pos = activeCam.mapPos(g_origin, model)
        GL.uniform3f(loc2, pos.x, pos.y, pos.z)
      }

      const loc3 = GL.getUniformLocation(this.program!, "u_omniLS_pos")
      if (loc3 && omniDirLS instanceof OmniDirLS && model instanceof Model) {
        const pos = omniDirLS.mapPos(g_origin, model)
        GL.uniform3f(loc3, pos.x, pos.y, pos.z)
      }

      const loc4 = GL.getUniformLocation(this.program!, "u_up_dir")
      if (loc4 && model instanceof Model) {
        const worldUp = new Vector1x4(0, 0, 1, 0)
        const modelUp = worldUp.mul(model.modelMatrix.inverse())
        GL.uniform3f(loc4, modelUp.x, modelUp.y, modelUp.z)
      }
    }
  }

  setUniformVariablesInFragShader(model: Model, material?: Material) {
    const currScene = getCurrScene()
    if (currScene && GL) {
      const { omniDirRGB: omniDirLS, lowerAmbientRGB: lowerAmbientLS, upperAmbientRGB: upperAmbientLS } = currScene

      const loc0 = GL.getUniformLocation(this.program!, "u_int")
      if (loc0) {
        GL.uniform3f(loc0, omniDirLS[0], omniDirLS[1], omniDirLS[2])
      }

      const loc1 = GL.getUniformLocation(this.program!, "u_gnd")
      if (loc1) {
        GL.uniform3f(loc1, lowerAmbientLS[0], lowerAmbientLS[1], lowerAmbientLS[2])
      }

      const loc2 = GL.getUniformLocation(this.program!, "u_sky")
      if (loc2) {
        GL.uniform3f(loc2, upperAmbientLS[0], upperAmbientLS[1], upperAmbientLS[2])
      }

      if (material) {
        const loc3 = GL.getUniformLocation(this.program!, "u_ambi")
        if (loc3) {
          GL.uniform3f(loc3, material.diff[0], material.diff[1], material.diff[2])
        }

        const loc4 = GL.getUniformLocation(this.program!, "u_diff")
        if (loc4) {
          const r = omniDirLS[0] * material.diff[0]
          const g = omniDirLS[1] * material.diff[1]
          const b = omniDirLS[2] * material.diff[2]
          GL.uniform3f(loc4, r, g, b)
        }

        const loc5 = GL.getUniformLocation(this.program!, "u_spec")
        if (loc5) {
          const r = omniDirLS[0] * material.spec[0]
          const g = omniDirLS[1] * material.spec[1]
          const b = omniDirLS[2] * material.spec[2]
          GL.uniform4f(loc5, r, g, b, material.shinyExponent)
        }

        const loc6 = GL.getUniformLocation(this.program!, "u_smpl")
        if (loc6 && material.textures[0]) {
          GL.activeTexture(GL.TEXTURE0)
          GL.bindTexture(GL.TEXTURE_2D, material.textures[0])
          GL.uniform1i(loc6, 0)
        }

        const loc7 = GL.getUniformLocation(this.program!, "u_norm")
        if (loc7 && material.textures[1]) {
          GL.activeTexture(GL.TEXTURE1)
          GL.bindTexture(GL.TEXTURE_2D, material.textures[1])
          GL.uniform1i(loc7, 1)
        }
      }

      const loc8 = GL.getUniformLocation(this.program!, "u_up_dir")
      if (loc8 && model instanceof Model) {
        const dir = g_up.mul(model.modelMatrix.inverse())
        GL.uniform3f(loc8, dir.x, dir.y, dir.z)
      }
    }
  }

  static get vertexAttributeDescs(): VertexAttributeDesc[] {
    return []
  }

  static get vertexElementCount(): number {
    return 0
  }
}
