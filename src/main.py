from __future__ import annotations
import pyray as raylib

V3  = raylib.Vector3
V4  = raylib.Vector4
TRS = raylib.Matrix
mul_v3 = raylib.vector3_multiply


def print_v3(vec: V3):
    print(f"Vec: X: {vec.x} | Y: {vec.y} | Z: {vec.z}")

def get_v3(x: float=0.0, y: float=0.0, z: float=0.0) -> V3:
    return raylib.Vector3(x, y, z)

def quantize_v3(vec: V3) -> V3:
    return get_v3(vec.x - (vec.x % 90), vec.y - (vec.y % 90), vec.z - (vec.z % 90))

def to_rad_vec3(vec: V3) -> V3:
    vec.x *= raylib.DEG2RAD
    vec.y *= raylib.DEG2RAD
    vec.z *= raylib.DEG2RAD
    return vec

class MrAngryCubeMoveBehaviour:
    def __init__(self, player: MrAngryCube):
        self._quarter_rotation = 0.0
        self._can_move = True
        self._velocity = 45    # degrees
        self._wait_time = 1.0  # seconds
        self.__last_move_time = raylib.get_time()

        self.player = player
        self.heading = get_v3(z=1)

        # self.next_heading = # FIXME

    def get_rotation_vector(self) -> V3:
        up_vector = get_v3(y=1)
        return raylib.vector3_cross_product(up_vector, self.heading)

    @property
    def rotation(self) -> V3:
        quat = raylib.quaternion_from_matrix(self.transform)
        euler_rad = raylib.quaternion_to_euler(quat)
        return get_v3(euler_rad.x * raylib.RAD2DEG, euler_rad.y * raylib.RAD2DEG, euler_rad.z * raylib.RAD2DEG)

    @property
    def position(self) -> V3:
        return self.player.position

    @property
    def transform(self):
        return self.player.transform

    @transform.setter
    def transform(self, new_transform: TRS):
        self.player.transform = new_transform

    def update(self):
        angle_step = raylib.get_frame_time() * self._velocity  # 90 degrees per second.
        if raylib.get_time() - self.__last_move_time >= self._wait_time:
            self._can_move = True
        else:
            return

        if self._quarter_rotation + angle_step - 90 > 0:
            self._can_move = False
            self._quarter_rotation = 0

            rot_vec: V3 = self.get_rotation_vector()
            rot_add: V3 = raylib.vector3_multiply(rot_vec, get_v3(90, 90, 90))
            r = raylib.vector3_add(rot_add, quantize_v3(self.rotation))

            self.transform = raylib.matrix_multiply(raylib.matrix_rotate_xyz(to_rad_vec3(r)), raylib.matrix_translate(self.position.x, self.position.y, self.position.z))
            self.__last_move_time = raylib.get_time()

        else:
            self._quarter_rotation += angle_step


        if self._can_move:
            initial_transform = raylib.matrix_translate(0, -self.player.size/2, 0)
            final_transform = raylib.matrix_translate(0, self.player.size/2, 0)

            self.transform = raylib.matrix_multiply(self.transform, initial_transform)

            mat_rot = raylib.matrix_rotate(self.get_rotation_vector(), angle_step * raylib.DEG2RAD)
            self.transform = raylib.matrix_multiply(self.transform, mat_rot)
            
            self.transform = raylib.matrix_multiply(self.transform, final_transform)


class MrAngryCube:
    def __init__(self, position: V3=get_v3(), velocity: V3=get_v3(), size:float=2.0):
        self.transform = raylib.matrix_identity()
        self.position = position
        self.velocity = velocity
        self.size = size
        self.behaviour = MrAngryCubeMoveBehaviour(self)

        texture = raylib.load_texture("/media/lyonbach/work/Projects/mr-angry-cube-raylib/textures/metal.png")
        shader =  raylib.load_shader(
            "/media/lyonbach/work/Projects/mr-angry-cube-raylib/shaders/base.vs",
            "/media/lyonbach/work/Projects/mr-angry-cube-raylib/shaders/base.fs"
            )

        self.model = raylib.load_model("/media/lyonbach/work/Projects/mr-angry-cube-raylib/models/mr_angry_cube.obj")
        # self.model = raylib.load_model("/media/lyonbach/work/Projects/mr-angry-cube-raylib/models/mr_angry_cube_high_res.obj")
        self.model.materials[0].shader = shader
        self.model.materials[0].maps[raylib.MATERIAL_MAP_DIFFUSE].texture = texture

    @property
    def position(self):
        return V3(self.transform.m12, self.transform.m13, self.transform.m14)

    @position.setter
    def position(self, new_position: V3):
        self.transform.m12, self.transform.m13, self.transform.m14 = new_position.x, new_position.y, new_position.z

    def update(self):
        self.behaviour.update()

    def draw(self):
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
