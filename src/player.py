import pyray as raylib

from game_object import IGameObject
from helpers import v3_quantize, v3_print


class IMoveBehaviour:
    def apply(self, game_object: "IGameObject"):
        raise NotImplementedError("The move method must be implemented by subclasses.")

class MacAngeryMoveBehaviour(IMoveBehaviour):
    def __init__(self):
        self._wait_time: float = 1.0
        self._last_move_at: float = raylib.get_time()
        self._is_moving: bool = True
        self._target_position: raylib.Vector3 = raylib.Vector3(1, 0, 0)

    def apply(self, game_object: "IGameObject"):
        frame_time = raylib.get_frame_time()
        if raylib.get_time() - self._last_move_at > self._wait_time:
            self._is_moving = True

        if self._is_moving:
            delta_position: raylib.Vector3 = raylib.vector3_multiply(game_object.velocity, raylib.Vector3(frame_time))
            calculated_position = raylib.vector3_add(game_object.position, delta_position)

            if raylib.vector3_distance(calculated_position, self._target_position) < 2 * frame_time:
                self._target_position: raylib.Vector3 = v3_quantize(game_object.position)
                game_object.position = self._target_position
                self._is_moving = False
                self._last_move_at = raylib.get_time()
            else:
                game_object.position = calculated_position

        v3_print(game_object.position)

class Player(IGameObject):
    def __init__(self):
        super().__init__()
        self._move_behaviour: IMoveBehaviour = MacAngeryMoveBehaviour()

    def draw(self):
        raylib.draw_cube(self.position, 1, 1, 1, raylib.RED)

    def update(self):
        self._move_behaviour.apply(self)

    def get_move_behaviour(self) -> IMoveBehaviour:
        return self._move_behaviour

    def set_move_behaviour(self, move_behaviour: IMoveBehaviour):
        self._move_behaviour = move_behaviour
