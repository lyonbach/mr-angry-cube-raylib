from mac_types import *
from mac_move_behaviour import MrAngryCubeMoveBehaviour
from mac_helpers import get_v3
from mac_texture_animation import TextureAnimation


class MrAngryCube:
    def __init__(self, position: V3=get_v3(), size:float=2.0):
        self.transform = raylib.matrix_identity()
        self.position = position
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

        self.animation = TextureAnimation(self.mat_face.shader, texture_face, 9)

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
