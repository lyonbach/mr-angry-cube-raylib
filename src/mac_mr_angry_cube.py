from typing import override
from mac_types import *
from mac_move_behaviour import MrAngryCubeMoveBehaviour
from mac_helpers import get_v3, get_game
from mac_texture_animation import TextureAnimation
from mac_game_object import GameObject

class MrAngryCube(GameObject):
    def __init__(self, name: str, position: V3=get_v3(), move_behaviour: MrAngryCubeMoveBehaviour=None, size:float=2.0):
        self.transform = raylib.matrix_identity()
        self.name = name
        self.position = position
        self.size = size
        self.behaviour = move_behaviour or MrAngryCubeMoveBehaviour(self)

        game = get_game()
        self.model = raylib.load_model(game.settings["assets"][name]["model_path"])
        assert self.model.meshCount >= 2, f"Expected >= 2 meshes in OBJ, found {self.model.meshCount}"

        body_shader = raylib.load_shader(
            game.settings["assets"][name]["body_vs"],
            game.settings["assets"][name]["body_fs"],
        )

        face_shader = raylib.load_shader(
            game.settings["assets"][name]["face_vs"],
            game.settings["assets"][name]["face_fs"],
        )

        texture_body = raylib.load_texture(game.settings["assets"][name]["body_tex"])
        self.mat_body = raylib.load_material_default()
        self.mat_body.shader = body_shader
        self.mat_body.maps[raylib.MATERIAL_MAP_DIFFUSE].texture = texture_body

        texture_face = raylib.load_texture(game.settings["assets"][name]["face_tex"])
        self.mat_face = raylib.load_material_default()
        self.mat_face.shader = face_shader
        self.mat_face.maps[raylib.MATERIAL_MAP_DIFFUSE].texture = texture_face

        self.animation = TextureAnimation(self.mat_face.shader, texture_face, 9)

    @override
    def update(self):
        self.behaviour.update()
        self.animation.update()

    @override
    def draw(self):
        raylib.draw_mesh(self.model.meshes[0], self.mat_body, self.transform)
        raylib.draw_mesh(self.model.meshes[1], self.mat_face, self.transform)
