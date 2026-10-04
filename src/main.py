from __future__ import annotations
from enum import Enum

import pyray as raylib

V3  = raylib.Vector3
V4  = raylib.Vector4
M4 = TRS = raylib.Matrix

mat_mul = raylib.matrix_multiply


def snap_vector_to_cardinal(v: V3) -> V3:
    ax, ay, az = abs(v.x), abs(v.y), abs(v.z)
    # Identify which cardinal axis is closest
    if ax >= ay and ax >= az:
        return V3(1.0 if v.x > 0 else -1.0, 0.0, 0.0)
    elif ay >= ax and ay >= az:
        return V3(0.0, 1.0 if v.y > 0 else -1.0, 0.0)
    else:
        return V3(0.0, 0.0, 1.0 if v.z > 0 else -1.0)

def snap_rotation_to_grid(mat: M4) -> M4:
    right = V3(mat.m0, mat.m1, mat.m2)
    up    = V3(mat.m4, mat.m5, mat.m6)

    snapped_right = snap_vector_to_cardinal(right)
    snapped_up    = snap_vector_to_cardinal(up)

    snapped_forward = raylib.vector3_cross_product(snapped_right, snapped_up)

    # 4. Write back the snapped rotation basis
    mat.m0, mat.m1, mat.m2 = snapped_right.x, snapped_right.y, snapped_right.z
    mat.m4, mat.m5, mat.m6 = snapped_up.x, snapped_up.y, snapped_up.z
    mat.m8, mat.m9, mat.m10 = snapped_forward.x, snapped_forward.y, snapped_forward.z
    return mat

def snap_translation_to_grid(mat: M4, grid_size: float = 1.0) -> M4:
    # m12, m13, m14 contain the XYZ world position
    mat.m12 = round(mat.m12 / grid_size) * grid_size
    mat.m13 = round(mat.m13 / grid_size) * grid_size
    mat.m14 = round(mat.m14 / grid_size) * grid_size
    return mat

def snap_matrix_to_grid(mat: M4, grid_size: float = 1.0) -> M4:
    mat = snap_rotation_to_grid(mat)
    mat = snap_translation_to_grid(mat, grid_size)
    return mat

def print_v3(vec: V3):
    print(f"Vec: X: {vec.x} | Y: {vec.y} | Z: {vec.z}")

def get_v3(x: float=0.0, y: float=0.0, z: float=0.0) -> V3:
    return V3(x, y, z)

def get_minus_v3(vec: V3) -> V3:
    return raylib.vector3_multiply(vec, get_v3(-1, -1, -1))

def to_rad_vec3(vec: V3) -> V3:
    vec.x *= raylib.DEG2RAD
    vec.y *= raylib.DEG2RAD
    vec.z *= raylib.DEG2RAD
    return vec


class Heading:
    NO_HEADING: V3 = get_v3()
    NORTH     : V3 = get_v3(x= 1.0)
    SOUTH     : V3 = get_v3(x=-1.0)
    EAST      : V3 = get_v3(z= 1.0)
    WEST      : V3 = get_v3(z=-1.0)

class Animation:
    def __init__(self, shader: raylib.Shader, texture: raylib.Texture, frame_count: int):
        self.shader = shader
        self.texture = texture
        self.frame_count = frame_count
        self._current_frame = 0

    def update(self):
        loc_texel = raylib.get_shader_location(self.shader, "texelSize")
        loc_move  = raylib.get_shader_location(self.shader, "moveBehaviourIndex")

        # Set texel size once based on the texture's resolution
        texel_data = raylib.ffi.new("float[2]", [1.0 / self.texture.width, 1.0 / self.texture.height])
        raylib.set_shader_value(self.shader, loc_texel, texel_data, raylib.ShaderUniformDataType.SHADER_UNIFORM_VEC2)

        # Update moveBehaviourIndex in your update loop whenever it changes
        current_behavior = raylib.ffi.new("float[1]", [4.0])
        raylib.set_shader_value(self.shader, loc_move, current_behavior, raylib.ShaderUniformDataType.SHADER_UNIFORM_FLOAT)

class MrAngryCubeMoveBehaviour:

    def __init__(self, player: MrAngryCube):
        self._quarter_rotation = 0.0
        self._can_move = True
        # self._velocity = 90    # degrees
        self._wait_time = .50  # seconds
        self._velocity  =  45  # degrees
        # self._wait_time =  1.0 # seconds
        self.__last_move_time = raylib.get_time()

        self.player = player
        self.heading = Heading.EAST
        self.next_heading = Heading.EAST
        self.update_pivot_point(self.next_heading)

    def update_pivot_point(self, heading: V3):
        self._pivot_point = get_minus_v3(self.position)
        offset_vector = raylib.vector3_multiply(get_v3(x=-self.player.size/2, z=-self.player.size/2), heading)
        offset_vector = raylib.vector3_add(offset_vector, get_v3(y=self.player.size/2))
        self._pivot_point = raylib.vector3_add(self._pivot_point, offset_vector)

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
        if self.heading == Heading.NO_HEADING:
            self.heading = self.next_heading
            return
        angle_step = raylib.get_frame_time() * self._velocity  # 90 degrees per second.
        if raylib.get_time() - self.__last_move_time >= self._wait_time:
            self._can_move = True
        else:
            return

        if self._quarter_rotation + angle_step - 90 > 0:
            self._can_move = False
            self._quarter_rotation = 0

            self.transform = snap_matrix_to_grid(self.transform)
            self.__last_move_time = raylib.get_time()
            self.heading = self.next_heading
            self.update_pivot_point(self.heading)
        else:
            self._quarter_rotation += angle_step

        if self._can_move:
            initial_transform = raylib.matrix_translate(self._pivot_point.x, self._pivot_point.y, self._pivot_point.z)
            self.transform = mat_mul(self.transform, initial_transform)

            mat_rot = raylib.matrix_rotate(self.get_rotation_vector(), angle_step * raylib.DEG2RAD)
            self.transform = mat_mul(self.transform, mat_rot)

            final_transform = raylib.matrix_translate(-self._pivot_point.x, -self._pivot_point.y, -self._pivot_point.z)
            self.transform = mat_mul(self.transform, final_transform)

class MrAngryCube:
    def __init__(self, position: V3=get_v3(), velocity: V3=get_v3(), size:float=2.0):
        self.transform = raylib.matrix_identity()
        self.position = position
        self.velocity = velocity
        self.size = size
        self.behaviour = MrAngryCubeMoveBehaviour(self)

        self.model = raylib.load_model("/media/lyonbach/work/Projects/mr-angry-cube-raylib/models/mr_angry_cube_1.obj")
        assert self.model.meshCount >= 2, f"Expected >= 2 meshes in OBJ, found {self.model.meshCount}"

        body_shader = raylib.load_shader(
            "/media/lyonbach/work/Projects/mr-angry-cube-raylib/shaders/mr-angry-cube-body.vs",
            "/media/lyonbach/work/Projects/mr-angry-cube-raylib/shaders/mr-angry-cube-body.fs"
        )

        face_shader = raylib.load_shader(
            "/media/lyonbach/work/Projects/mr-angry-cube-raylib/shaders/mr-angry-cube-face.vs",
            "/media/lyonbach/work/Projects/mr-angry-cube-raylib/shaders/mr-angry-cube-face.fs"
        )

        texture_body = raylib.load_texture("/media/lyonbach/work/Projects/mr-angry-cube-raylib/textures/test_1.png")
        self.mat_body = raylib.load_material_default()
        self.mat_body.shader = body_shader
        self.mat_body.maps[raylib.MATERIAL_MAP_DIFFUSE].texture = texture_body

        # texture_face = raylib.load_texture("/media/lyonbach/work/Projects/mr-angry-cube-raylib/textures/mr-angry-cube-face-0.png")
        texture_face = raylib.load_texture("/media/lyonbach/work/Projects/mr-angry-cube-raylib/textures/33ad35f1.png")
        self.mat_face = raylib.load_material_default()
        self.mat_face.shader = face_shader
        self.mat_face.maps[raylib.MATERIAL_MAP_DIFFUSE].texture = texture_face

        self.animation = Animation(self.mat_face.shader, texture_face, 9)

    @property
    def position(self):
        return V3(self.transform.m12, self.transform.m13, self.transform.m14)

    @position.setter
    def position(self, new_position: V3):
        self.transform.m12, self.transform.m13, self.transform.m14 = new_position.x, new_position.y, new_position.z

    def update(self):
        self.behaviour.update()
        self.animation.update()

    def draw(self):
        raylib.draw_mesh(self.model.meshes[0], self.mat_body, self.transform)
        raylib.draw_mesh(self.model.meshes[1], self.mat_face, self.transform)

class FollowCamera:
    def __init__(self, player: MrAngryCube, offset: V3=get_v3(5, 5, 5), fovy: float=45):
        self._offset = offset
        self._player = player
        self._camera = raylib.Camera3D()
        self._camera.fovy = fovy
        self._camera.up = get_v3(y=1)
        self._camera.projection = raylib.CameraProjection.CAMERA_PERSPECTIVE

        self.target = player.position
        self.position = raylib.vector3_add(self._camera.target, self._offset)

    def get(self):
        return self._camera

    @property
    def target(self) -> V3:
        return self._camera.target

    @target.setter
    def target(self, new_target: V3):
        self._camera.target = new_target

    @property
    def position(self) -> V3:
        return self._camera.position

    @position.setter
    def position(self, new_position: V3):
        self._camera.position = new_position

    def update(self):
        new_position = raylib.vector3_add(self._player.position, self._offset)
        difference = raylib.vector3_subtract(new_position, self.position)
        self.position = raylib.vector3_add(self.position, raylib.vector3_scale(difference, .025))
        self.position.y = self._offset.y
        self.target = self._player.position


class KeyEventsListener:
    def __init__(self, player: MrAngryCube):
        self._player = player

    def update(self):
        if raylib.is_key_pressed(raylib.KEY_W):
            self._player.behaviour.next_heading = Heading.EAST
        elif raylib.is_key_pressed(raylib.KEY_S):
            self._player.behaviour.next_heading = Heading.WEST
        elif raylib.is_key_pressed(raylib.KEY_D):
            self._player.behaviour.next_heading = Heading.NORTH
        elif raylib.is_key_pressed(raylib.KEY_A):
            self._player.behaviour.next_heading = Heading.SOUTH
        elif raylib.is_key_pressed(raylib.KEY_Q):
            self._player.behaviour.next_heading = Heading.NO_HEADING
        elif raylib.is_key_pressed(raylib.KEY_SPACE):
            self._player.behaviour._can_move = True

def main():
    # Initialization
    screen_width = 800
    screen_height = 800

    raylib.init_window(screen_width, screen_height, "raylib [python] example - basic window")

    raylib.set_target_fps(60)  # Set our game to run at 60 frames-per-second


    player = MrAngryCube(get_v3(y=1.0))
    camera = FollowCamera(player, get_v3(10, 10, 10))

    key_events_listener = KeyEventsListener(player)

    # Main game loop
    while not raylib.window_should_close():  # Detect window close button or ESC key
        # player.behaviour.heading = Heading.NO_HEADING
        # Update
        raylib.begin_drawing()
        raylib.clear_background(raylib.DARKBLUE)

        # Draw
        raylib.begin_mode_3d(camera.get())
        raylib.draw_grid(20, 1.0)

        key_events_listener.update()

        player.update()
        player.draw()

        camera.update()

        raylib.end_mode_3d()

        raylib.end_drawing()
    # De-Initialization
    raylib.close_window()  # Close window and OpenGL context


main()
