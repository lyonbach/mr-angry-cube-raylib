from mac_types import *
from mac_helpers import get_v3, get_game

class IGameObject:
    def __init__(self, name: str, position: V3=get_v3(), size:float=1.0):
        self.name = name
        self.transform = raylib.matrix_identity()
        self.position = position
        self.size = size

    @property
    def position(self):
        return V3(self.transform.m12, self.transform.m13, self.transform.m14)

    @position.setter
    def position(self, new_position: V3):
        self.transform.m12, self.transform.m13, self.transform.m14 = new_position.x, new_position.y, new_position.z

    def update(self):
        raise NotImplementedError

    def draw(self):
        raise NotImplementedError

class GameObject(IGameObject):
    def __init__(self, name, position = get_v3(), size = 1):
        super().__init__(name, position, size)

        game = get_game()
        self.model = raylib.load_model(game.settings["assets"][name]["model_path"])

        shader = raylib.load_shader(
            game.settings["assets"][name]["main_vs"],
            game.settings["assets"][name]["main_fs"],
        )

        texture = raylib.load_texture(game.settings["assets"][name]["main_tex"])
        self.material = raylib.load_material_default()
        self.material.shader = shader
        self.material.maps[raylib.MATERIAL_MAP_DIFFUSE].texture = texture

    def update(self):
        pass

    def draw(self):
        raylib.draw_mesh(self.model.meshes[0], self.material, self.transform)