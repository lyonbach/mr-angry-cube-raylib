from __future__ import annotations
from mac_types import *
from mac_helpers import get_v3


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

