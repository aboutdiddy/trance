#ifndef TRANCE_DIRECTOR_H
#define TRANCE_DIRECTOR_H

#include <windows.h>
#undef min
#undef max

#include <cstddef>
#include <memory>
#include <vector>
#include <GL/glew.h>
#include <OVR_CAPI.h>
#include <OVR_CAPI_GL.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "font.h"

namespace trance_pb {
  class Session;
  class Program;
}

struct Font;
class Image;
class ThemeBank;
class Visual;
class Director {
public:

  Director(sf::RenderWindow& window, const trance_pb::Session& session,
           ThemeBank& themes, const trance_pb::Program& program);
  ~Director();

  // Called from play_session() in main.cpp.
  void set_program(const trance_pb::Program& program);
  void update();
  void render() const;

  // Visual API: called from Visual objects to render and control the
  // various elements.
  Image get_image(bool alternate = false) const;
  const std::string& get_text(bool alternate = false) const;
  void maybe_upload_next() const;

  enum class Anim {
    NONE,
    ANIM,
    ANIM_ALTERNATE,
  };
  void render_animation_or_image(
      Anim type, const Image& image,
      float alpha, float multiplier = 8.f, float zoom = 0.f) const;
  void render_image(const Image& image, float alpha,
                    float multiplier = 8.f, float zoom = 0.f) const;
  void render_text(const std::string& text, float multiplier = 4.f) const;
  void render_subtext(float alpha, float multiplier = 6.f) const;
  void render_spiral() const;

  void rotate_spiral(float amount);
  void change_spiral();
  void change_font(bool force = false);
  void change_subtext(bool alternate = false);
  bool change_themes();
  void change_visual();

private:

  void init_oculus_rift();
  sf::Vector2f off3d(float multiplier, bool text) const;
  uint32_t view_width() const;

  void render_texture(float l, float t, float r, float b,
                      bool flip_h, bool flip_v) const;
  void render_raw_text(const std::string& text, const Font& font,
                       const sf::Color& colour, const sf::Vector2f& offset = {},
                       float scale = 1.f) const;
  sf::Vector2f get_text_size(const std::string& text, const Font& font) const;

  sf::RenderWindow& _window;
  const trance_pb::Session& _session;
  ThemeBank& _themes;
  FontCache _fonts;
  uint32_t _width;
  uint32_t _height;
  const trance_pb::Program* _program;

  struct {
    bool enabled;
    ovrHmd hmd;

    uint32_t fbo;
    uint32_t fb_tex;
    uint32_t fb_depth;

    union ovrGLConfig gl_cfg;
    ovrGLTexture fb_ovr_tex[2];
    ovrEyeRenderDesc eye_desc[2];

    mutable bool rendering_right;
  } _oculus;

  GLuint _image_program;
  GLuint _spiral_program;
  GLuint _text_program;
  GLuint _quad_buffer;
  GLuint _tex_buffer;

  float _spiral;
  uint32_t _spiral_type;
  uint32_t _spiral_width;
  std::string _current_font;
  std::string _current_subfont;
  std::vector<std::string> _subtext;

  uint32_t _switch_themes;
  std::unique_ptr<Visual> _visual;
  std::unique_ptr<Visual> _old_visual;

};

#endif