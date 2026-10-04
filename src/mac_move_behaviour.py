from __future__ import annotations
from mac_types import *

from mac_helpers import Heading, get_v3, get_minus_v3, snap_matrix_to_grid



class MrAngryCubeMoveBehaviour:

    def __init__(self, player: MrAngryCube):
        self._quarter_rotation = 0.0
        self._can_move = True
        self._wait_time = .50  # seconds
        self._velocity  =  45  # degrees
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
    def transform(self, new_transform: M4):
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
