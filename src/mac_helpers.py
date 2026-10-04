from mac_types import *

def get_v3(x: float=0.0, y: float=0.0, z: float=0.0) -> V3:
    return V3(x, y, z)

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

