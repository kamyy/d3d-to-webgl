uniform mat4 u_model_view_proj_matrix; // model to view to projection space transform
/*
uniform vec4 u_camera_pos; // camera pos in model space
uniform vec4 u_omniLS_pos; // omniLS pos in model space
uniform vec4 u_up_dir; // world up in model space
*/

attribute vec4 a_pos;
attribute vec4 a_col;

varying vec4 v_col;

void main() {
    gl_Position = a_pos * u_model_view_proj_matrix;
    v_col       = a_col;
}

