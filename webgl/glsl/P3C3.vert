uniform mat4 u_model_view_proj_matrix; // model to view to projection space transform

attribute vec4 a_pos;
attribute vec4 a_col;

varying vec4 v_col;

void main() {
    gl_Position = u_model_view_proj_matrix * a_pos;
    v_col       = a_col;
}

