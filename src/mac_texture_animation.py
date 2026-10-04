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

        # Set texel size once based on the texture's resolution
        uv_scale_data  = raylib.ffi.new("float[2]", [0, 0])  # FIXME
        raylib.set_shader_value(self.shader, loc_uv_scale, uv_scale_data, raylib.ShaderUniformDataType.SHADER_UNIFORM_VEC2)

        # Update moveBehaviourIndex in your update loop whenever it changes
        uv_offset_data = raylib.ffi.new("float[2]", [0, 0]) # FIXME
        raylib.set_shader_value(self.shader, loc_uv_offset, uv_offset_data, raylib.ShaderUniformDataType.SHADER_UNIFORM_VEC2)
