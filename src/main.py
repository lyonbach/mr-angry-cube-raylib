from __future__ import annotations
import pyray as raylib

V3 = raylib.Vector3
mul_v3 = raylib.vector3_multiply


def print_v3(vec: raylib.Vector3):
    print(f"Vec: X: {vec.x} | Y: {vec.y} | Z: {vec.z}")

def get_v3(x: float=0.0, y: float=0.0, z: float=0.0):
    return raylib.Vector3(x, y, z)

class MrAngryCubeMoveBehaviour:
    def __init__(self, player: MrAngryCube):
        self.player = player
        self.heading = get_v3(z=1)
        # self.next_heading = # FIXME

    def get_rotation_vector(self) -> V3:
        up_vector = get_v3(y=1)
        return raylib.vector3_cross_product(up_vector, self.heading)

    def update(self):
        # mat_rot = raylib.matrix_rotate()
        print_v3(self.get_rotation_vector())


class MrAngryCube:
    def __init__(self, position: V3=get_v3(), velocity: V3=get_v3(), size:float=2.0):
        self.transform = raylib.Matrix()
        self.position = position
        self.velocity = velocity
        self.size = size
        self.behaviour = MrAngryCubeMoveBehaviour(self)

        self.model = raylib.load_model("/media/lyonbach/work/Projects/mr-angry-cube-raylib/models/mr_angry_cube.obj")
        self.model.materialCount = 2
        self.model.meshCount = 2
        self.model.materials[0] = raylib.load_material_default()
        self.model.materials[0].tex

    @property
    def position(self):
        return V3(self.transform.m12, self.transform.m13, self.transform.m14)

    @position.setter
    def position(self, new_position: V3):
        self.transform.m12, self.transform.m13, self.transform.m14 = new_position.x, new_position.y, new_position.z

    def update(self):
        self.behaviour.update()

    def draw(self):
        # raylib.draw_cube(self.position, self.size, self.size, self.size, raylib.DARKGREEN)
        raylib.draw_mesh(self.model.meshes[0], self.model.materials[0], self.transform)



def main():
    # Initialization
    screen_width = 1024
    screen_height = 1024

    raylib.init_window(screen_width, screen_height, "raylib [python] example - basic window")

    raylib.set_target_fps(60)  # Set our game to run at 60 frames-per-second


    camera = raylib.Camera3D()
    camera.projection = raylib.CameraProjection.CAMERA_PERSPECTIVE
    camera.fovy = 45
    camera.position = get_v3(10, 10, 10)
    camera.target = get_v3()
    camera.up = get_v3(y=1)

    player = MrAngryCube(get_v3(y=1.0))

    # Main game loop
    while not raylib.window_should_close():  # Detect window close button or ESC key
        # Update
        raylib.begin_drawing()
        raylib.clear_background(raylib.BLACK)

        # Draw
        raylib.begin_mode_3d(camera)
        raylib.draw_grid(20, 1.0)

        player.update()
        player.draw()

        raylib.end_mode_3d()

        raylib.end_drawing()
    # De-Initialization
    raylib.close_window()  # Close window and OpenGL context

main()
