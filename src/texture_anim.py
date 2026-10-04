import pyray as raylib

width, height = 800, 800

def main():
    raylib.init_window(width, height, "Animation Test")
    raylib.set_target_fps(60)  # Set our game to run at 60 frames-per-second

    shader = raylib.load_shader(
        "/media/lyonbach/work/Projects/mr-angry-cube-raylib/shaders/mr-angry-cube-face.vs",
        "/media/lyonbach/work/Projects/mr-angry-cube-raylib/shaders/mr-angry-cube-face.fs"
    )

    texture = raylib.load_texture(
        "/media/lyonbach/work/Projects/mr-angry-cube-raylib/textures/33ad35f1.png"
        # "/media/lyonbach/work/Projects/mr-angry-cube-raylib/textures/texel_checker_crayon.png"
    )

    model = raylib.load_model("/media/lyonbach/work/Projects/mr-angry-cube-raylib/models/tileSmall.obj")

    # mesh = raylib.gen_mesh_plane(1, 1, 2, 2)
    mesh = model.meshes[0]

    material = raylib.load_material_default()
    material.shader = shader
    material.maps[raylib.MATERIAL_MAP_DIFFUSE].texture = texture


    transform = raylib.matrix_multiply(
        raylib.matrix_scale(1, 1, 1),
        raylib.matrix_translate(0, 0, 0)
    )

    camera = raylib.Camera3D()
    camera.fovy = 90
    camera.projection = raylib.CameraProjection.CAMERA_PERSPECTIVE
    camera.up = raylib.Vector3(0, 1, 0)
    camera.position = raylib.Vector3(0, 2, 2)
    camera.target = raylib.Vector3(0, 0, 0)

    uv_offsets = [
        [0, 0], [341, 0], [682, 0],
        [0, 341], [341, 341], [682, 341],
        [0, 682], [341, 682], [682, 682]
    ]
    current_frame = 0
    total_frames = 9
    last_frame_change = raylib.get_time()

    def set_uv_scale(value=[1.0, 1.0]):
        loc_float = raylib.get_shader_location(shader, "uvScale")
        float_value = raylib.ffi.new("float[2]", value)
        raylib.set_shader_value(shader, loc_float, float_value, raylib.ShaderUniformDataType.SHADER_UNIFORM_VEC2)

    def set_uv_offset(value=[1.0, 1.0]):
        loc_vec2 = raylib.get_shader_location(shader, "uvOffset")
        vec2_value = raylib.ffi.new("float[2]", value)
        raylib.set_shader_value(shader, loc_vec2, vec2_value, raylib.ShaderUniformDataType.SHADER_UNIFORM_VEC2)


    set_uv_scale([1/3, 1/3])
    while not raylib.window_should_close():
        raylib.begin_drawing()
        raylib.begin_mode_3d(camera)
        raylib.clear_background(raylib.BLACK)

        if raylib.get_time() - last_frame_change > 1/5:
            value = uv_offsets[current_frame]
            print("setting...")
            print(value)
            set_uv_offset(value)
            current_frame += 1
            current_frame = current_frame % total_frames
            last_frame_change = raylib.get_time()

        raylib.draw_mesh(mesh, material, transform)

        raylib.end_mode_3d()
        raylib.end_drawing()

    raylib.close_window()


main()
