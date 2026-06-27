import Camera from "../sceneGraph/Camera"
import Vector1x4 from "../math/Vector1x4"

const TXYZ_SCALAR = 0.01
const RXYZ_SCALAR = 0.25
const ZOOM_SCALAR = 0.05
const WHEEL_ZOOM_SCALAR = 0.002

function degreesToRadians(degrees: number) {
  return (degrees * Math.PI) / 180.0
}

type CameraControlScene = {
  activeCamera: Camera
  requestDrawScene(): void
}

export default class CameraControls {
  private lButtonDown = false
  private mButtonDown = false
  private rButtonDown = false
  private lx = 0
  private ly = 0

  constructor(
    private readonly canvas: HTMLCanvasElement,
    private readonly getScene: () => CameraControlScene | null
  ) {
    this.canvas.oncontextmenu = (event) => event.preventDefault()
    this.canvas.onmousedown = this.onMouseDown
    this.canvas.onwheel = this.onWheel
    window.onmousemove = this.onMouseMove
    window.onmouseup = this.onMouseUp
  }

  readonly onMouseDown = (event: MouseEvent) => {
    const rect = this.canvas.getBoundingClientRect()
    const x = event.clientX
    const y = event.clientY

    if (x > rect.left && x < rect.right && y > rect.top && y < rect.bottom) {
      switch (event.button) {
        case 0:
          this.lButtonDown = true
          break
        case 1:
          this.mButtonDown = true
          break
        case 2:
          this.rButtonDown = true
          break
        default:
          break
      }
      this.lx = x
      this.ly = y
    }
  }

  readonly onMouseUp = (event: MouseEvent) => {
    switch (event.button) {
      case 0:
        this.lButtonDown = false
        break
      case 1:
        this.mButtonDown = false
        break
      case 2:
        this.rButtonDown = false
        break
      default:
        break
    }
  }

  readonly onMouseMove = (event: MouseEvent) => {
    if (this.lButtonDown || this.mButtonDown || this.rButtonDown) {
      const scene = this.getScene()

      if (scene) {
        const camera = scene.activeCamera
        const x = event.clientX
        const y = event.clientY
        const target = camera.parent

        if (this.mButtonDown) {
          camera.translate(new Vector1x4(0, -(y - this.ly) * ZOOM_SCALAR, 0))
          this.lx = x
          this.ly = y
          scene.requestDrawScene()
        } else if (this.rButtonDown) {
          const dx = (this.lx - x) * TXYZ_SCALAR
          const dz = (y - this.ly) * TXYZ_SCALAR
          const dv = camera.mapPos(new Vector1x4(dx, 0, dz, 0), target ?? undefined)
          if (target) {
            target.translate(dv)
          }
          this.lx = x
          this.ly = y
          scene.requestDrawScene()
        } else if (this.lButtonDown) {
          target?.rotateZ(degreesToRadians(this.lx - x) * RXYZ_SCALAR)
          camera.rotateX(degreesToRadians(this.ly - y) * RXYZ_SCALAR, target ?? undefined)
          this.lx = x
          this.ly = y
          scene.requestDrawScene()
        }
      }
    }
  }

  readonly onWheel = (event: WheelEvent) => {
    const scene = this.getScene()
    if (!scene) {
      return
    }

    event.preventDefault()

    const camera = scene.activeCamera
    camera.translate(new Vector1x4(0, -event.deltaY * WHEEL_ZOOM_SCALAR, 0))
    scene.requestDrawScene()
  }
}
