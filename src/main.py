import pyray as raylib
from player import Player


raylib.init_window(800, 450, "My raylib window")
raylib.set_target_fps(60)

game_object = Player()
game_object.velocity = raylib.Vector3(1, 0, 0)
game_objects = [game_object]

camera = raylib.Camera3D()
camera.fovy = 45.0
camera.position = raylib.Vector3(10, 10, 10)
camera.target = raylib.Vector3(0, 0, 0)
camera.up = raylib.Vector3(0, 1, 0)
camera.projection = raylib.CameraProjection.CAMERA_PERSPECTIVE


while not raylib.window_should_close():
    raylib.begin_drawing()
    raylib.begin_mode_3d(camera)
    raylib.clear_background(raylib.BLACK)
    raylib.draw_grid(20, 0.5)

    for obj in game_objects:
        obj.update()
        obj.draw()

    raylib.draw_text("Hello, raylib!", 300, 210, 20, raylib.DARKGRAY)
    raylib.end_mode_3d()
    raylib.end_drawing()

raylib.close_window()
