import RefFrame from "./RefFrame"
import { SceneNodeJson } from "../types/scene"

export default class OmniDirLS extends RefFrame {
  color: [number, number, number]
  coeff0: number
  coeff1: number
  coeff2: number

  constructor(parent: RefFrame, node: SceneNodeJson) {
    super(parent, node)
    this.color = node.color ?? [1, 1, 1]
    this.coeff0 = node.coeff0 ?? 1
    this.coeff1 = node.coeff1 ?? 0
    this.coeff2 = node.coeff2 ?? 0
  }
}
