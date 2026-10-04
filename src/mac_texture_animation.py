from mac_types import *

class TextureAnimation:
    def __init__(self, shader: raylib.Shader, texture: raylib.Texture, frame_count: int):
        self.shader = shader
        self.texture = texture
        self.frame_count = frame_count
        self._current_frame = 0

    def update(self):
        loc_uv_scale  = raylib.get_shader_location(self.shader, "uvScale")
        loc_uv_offset = raylib.get_shader_location(self.shader, "uvOffset")

        # Set uv scale for 3x3 grid
        uv_scale_data  = raylib.ffi.new("float[2]", [1/3, 1/3])
        raylib.set_shader_value(self.shader, loc_uv_scale, uv_scale_data, raylib.ShaderUniformDataType.SHADER_UNIFORM_VEC2)

        # Update current frame based on time (10 fps)
        self._current_frame = int(raylib.get_time() * 10) % self.frame_count
        
        # Calculate offset (row-major)
        offset_x = float(self._current_frame % 3)
        offset_y = float(self._current_frame // 3)
        
        uv_offset_data = raylib.ffi.new("float[2]", [offset_x, offset_y])
        raylib.set_shader_value(self.shader, loc_uv_offset, uv_offset_data, raylib.ShaderUniformDataType.SHADER_UNIFORM_VEC2)
