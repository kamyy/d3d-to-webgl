import Shader from "../shader/Shader"

export type ShaderProgramName = "P3C3" | "P3N3" | "P3N3T2" | "P3N3B3T2"

export interface AppWebGLContext extends WebGLRenderingContext {
  mapOfShaders: Map<ShaderProgramName, Shader>
}
