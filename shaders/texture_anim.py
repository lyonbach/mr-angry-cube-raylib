import pyray as raylib

width, height = 800, 800

def main():
    raylib.init_window(width, height, "Animation Test")
    raylib.set_target_fps(60)  # Set our game to run at 60 frames-per-second

    shader = raylib.load_shader(
        "/media/lyonbach/work/Projects/mr-angry-cube-raylib/shaders/mr-angry-cube-face.vs",
        "/media/lyonbach/work/Projects/mr-angry-cube-raylib/shaders/mr-angry-cube-face.fs"
    )
    # shader = raylib.load_shader(
    #     "/media/lyonbach/work/Projects/mr-angry-cube-raylib/shaders/mr-angry-cube-body.vs",
    #     "/media/lyonbach/work/Projects/mr-angry-cube-raylib/shaders/mr-angry-cube-body.fs"
    # )

    texture = raylib.load_texture(
        "/media/lyonbach/work/Projects/mr-angry-cube-raylib/textures/texel_checker_crayon.png"
    )

    model = raylib.load_model("/media/lyonbach/work/Projects/mr-angry-cube-raylib/models/tileSmall.obj")

    # mesh = raylib.gen_mesh_plane(1, 1, 2, 2)
    mesh = model.meshes[0]

    material = raylib.load_material_default()
    material.shader = shader
    material.maps[raylib.MATERIAL_MAP_DIFFUSE].texture = texture

    loc_float = raylib.get_shader_location(shader, "moveBehaviourIndex")
    float_value = raylib.ffi.new("float[1]", [1.0])
    raylib.set_shader_value(shader, loc_float, float_value, raylib.ShaderUniformDataType.SHADER_UNIFORM_FLOAT)

    loc_vec2 = raylib.get_shader_location(shader, "texelSize")
    vec2_value = raylib.ffi.new("float[2]", [1, 1])
    raylib.set_shader_value(shader, loc_vec2, vec2_value, raylib.ShaderUniformDataType.SHADER_UNIFORM_VEC2)

    transform = raylib.matrix_multiply(raylib.matrix_scale(1, 1, 1), raylib.matrix_translate(0, 0, 0))

    camera = raylib.Camera3D()
    camera.fovy = 90
    camera.projection = raylib.CameraProjection.CAMERA_PERSPECTIVE
    camera.up = raylib.Vector3(0, 1, 0)
    camera.position = raylib.Vector3(0, 1, 1)
    camera.target = raylib.Vector3(0, 0, 0)

    while not raylib.window_should_close():
        raylib.begin_drawing()
        raylib.begin_mode_3d(camera)

        raylib.clear_background(raylib.BLACK)
        raylib.draw_mesh(mesh, material, transform)

        raylib.end_mode_3d()
        raylib.end_drawing()

    raylib.close_window()


main()
