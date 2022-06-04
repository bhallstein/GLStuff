#import "AppDelegate.h"
#import "OGLW/W.h"
#include "vectors.hpp"
#include "Camera.hpp"
#include "Lights.hpp"
#include "Primitives.hpp"
#include "FilePaths_CPP.hpp"
#include "TexImage.hpp"
#include "ObjImport.hpp"
#include "GLProg.hpp"
#include "TextTex.hpp"
#include "Renderer_2D_ColourIndexed.hpp"
#include "Renderer_LinearGradient.hpp"
#include "Renderer_2D_Textured.hpp"
#include "Renderer_3D_1L_UniformCol.hpp"
#include "Renderer_3D_1L_UniformCol_Instanced.hpp"
#include "Renderer_3D_1L_ColourIndexed.hpp"
#include "Renderer_3D_1L_ColourIndexed_Instanced.hpp"
#include "Renderer_3D_1L_Textured.hpp"
#include "Renderer_3D_1L_Textured_Instanced.hpp"
#include "Renderer_3D_1L_ReflectionMapped.hpp"
#include "Renderer_PixelPerfect.hpp"
#include "Renderer_PixelCol.hpp"
#include "Renderer_Text.hpp"
#include "renderer.hpp"

#define PROBABILITY(x) if (rand() / (float)RAND_MAX < x)
#define OTHERWISE      else

#define NEXT_WINDOW do { \
  if (wX + 2*defaultWinW > scrW) { \
    wX = 0; \
    wY += defaultWinH + 22; \
  } \
  else { \
    wX += defaultWinW; \
  } \
} while(0)

std::map<std::string, bool> tests_enabled = {
  {"2d_ci", true},
  {"lingrad", true},
  {"2d_tex", true},
  {"3d_uniformcol", true},
  {"3d_uniformcol_inst", true},
  {"3d_ci", true},
  {"3d_ci_inst", true},
  {"3d_t", true},
  {"3d_t_inst", true},
  {"3d_rm", true},
  {"pp", true},
  {"pc", true},
  {"text", true},
};

@interface AppDelegate () {
  std::map<std::string, W::Window*> windows;

  Renderer_2D_ColourIndexed *rend_2D_CI;
  Renderer_LinearGradient   *rend_LinGrad;
  Renderer_2D_Textured      *rend_2D_Tex;
  Renderer_3D_1L_UniformCol              *rend_3D_UniformCol;
  Renderer_3D_1L_UniformCol_Instanced    *rend_3D_UniformCol_Inst;
  Renderer_3D_1L_ColourIndexed           *rend_3D_CI;
  Renderer_3D_1L_ColourIndexed_Instanced *rend_3D_CI_Inst;
  Renderer_3D_1L_Textured                *rend_3D_T;
  Renderer_3D_1L_Textured_Instanced      *rend_3D_T_Inst;
  Renderer_3D_1L_ReflectionMapped        *rend_3D_RM;
  Renderer_PixelPerfect *rend_pp;
  Renderer_PixelCol     *rend_pc;
  Renderer_Text         *rend_text;

  DirectionalLight light;
  Camera cam_3D;

  size_t sprite_heart;
  int i;
}
@property NSTimer *gameTimer;

@end


@implementation AppDelegate

-(IBAction) setR:(NSSlider*)sender { /*rend_3D_1L_UniformCol->setR(sender.floatValue);*/ }
-(IBAction) setG:(NSSlider*)sender { /*rend_3D_1L_UniformCol->setG(sender.floatValue);*/ }
-(IBAction) setB:(NSSlider*)sender { /*rend_3D_1L_UniformCol->setB(sender.floatValue);*/ }
-(IBAction) setRatio:(NSSlider*)sender { /*rend_3D_1L_UniformCol->setRatio(sender.floatValue);*/ }

//float getCamAngleForHeight(float y) {
//  float minAngle    = 40.0, maxAngle = 70.0;
//  float angle_scale = (y - 1.0) / 9.0;
//  float new_angle   = (maxAngle - minAngle) * angle_scale;
//  return -new_angle;
//}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
  // Init cam
  float initial_cam_height = 2.5;
  cam_3D.pos = {0.0, initial_cam_height, 6.25};
  cam_3D.pos_target = {0, 0, 0};
  //  setOrientation(getCamAngleForHeight(initial_cam_height), {1.0, 0, 0});

  // Set up light
  float one_over_root_3 = 0.577;
  light.lightVector[0] = -one_over_root_3;
  light.lightVector[1] = -one_over_root_3;
  light.lightVector[2] = -one_over_root_3;
  light.lightProperties[0] = 0.9;
  light.lightProperties[1] = 0.0;
  light.lightProperties[2] = 0.0;

  //  float frustW = windowW / pixelsToWorldUnits;
  //  float frustH = windowH / pixelsToWorldUnits;
  //  cam.setOrthographic(-frustW*0.5, frustW*0.5, -frustH*0.5, frustH*0.5, 0.01, 1000.0);

  float defaultWinW = 300.0;
  float defaultWinH = 240.0;

  // Window & renderer setup
  W::Multisampling::T msLevel = W::Multisampling::X4;
  for (auto item : tests_enabled) {
    const std::string name = item.first;
    windows[item.first] = new W::Window(defaultWinW, defaultWinH, name.c_str(), 0, false, 0, msLevel);
  }

  int scrW;
  int scrH;
  W::getScreenSize(0, &scrW, &scrH);

  int wX = 0;
  int wY = 23;

  ObjFile obj_penguin = ObjFile::load(bundledFilePath("Assets/Penguin/Penguin.obj"));
  TexImage texIm_penguin = loadPngTexture(bundledFilePath("Assets/Penguin/penguin_box.png"));


  // 2D Colour Indexed
  if (tests_enabled["2d_ci"]) {
    W::Window *win = windows["2d_ci"];
    win->setPos(wX, wY);
    win->makeCurrentContext();
    rend_2D_CI = new Renderer_2D_ColourIndexed;
    rend_2D_CI->dither = true;
    rend_2D_CI->setUp();

    TexImage texImage = get_textTex("Muffins", 200, 100, 40, {0, 0, 0, 1}, {1, 1, 1, 1});
    unsigned int tex = Texture::create();
    Texture::bind(0, tex);
    Texture::upload(texImage.w, texImage.h, texImage.data, Texture::FilterLinear);
    free(texImage.data);

    win->clearCurrentContext();
    NEXT_WINDOW;
  }


  // Linear Gradient
  if (tests_enabled["lingrad"]) {
    W::Window *win = windows["lingrad"];
    win->setPos(wX, wY);
    win->makeCurrentContext();
    rend_LinGrad = new Renderer_LinearGradient;
    rend_LinGrad->dither = true;
    rend_LinGrad->setUp();
    v4 col_top = { 5./255, 117./255, 137./255, 1.0 };
    v4 col_btm = { 118./255, 221./255, 225./255, 1.0 };
    rend_LinGrad->cTL = col_top;
    rend_LinGrad->cTR = col_top;
    rend_LinGrad->cBL = col_btm;
    rend_LinGrad->cBR = col_btm;
    rend_LinGrad->needs_upload = true;

    win->clearCurrentContext();
    NEXT_WINDOW;
  }


  // 2D Textured
  if (tests_enabled["2d_tex"]) {
    W::Window *win = windows["2d_tex"];
    win->setPos(wX, wY);
    win->makeCurrentContext();
    rend_2D_Tex = new Renderer_2D_Textured;

    TexImage texImage = loadPngTexture(bundledFilePath("Assets/Skybox/back.png"));
    unsigned int tex = Texture::create();
    Texture::bind(0, tex);
    Texture::upload(texImage.w, texImage.h, texImage.data, Texture::FilterLinear);
    free(texImage.data);

    rend_2D_Tex->setUp();
    rend_2D_Tex->tex = tex;

    Primitive2D r = Rectangle2D() * 2.f;
    VBO::bind(rend_2D_Tex->buffers.vertexPos, VBO::Array);
    VBO::upload(r, VBO::Array, VBO::Static);

    Primitive2D t = Rectangle2DTextureCoords();
    VBO::bind(rend_2D_Tex->buffers.texCoord, VBO::Array);
    VBO::upload(t, VBO::Array, VBO::Static);

    rend_2D_Tex->n_vertices = 6;

    win->clearCurrentContext();
    NEXT_WINDOW;
  }


  // 3D Uniform Colour
  if (tests_enabled["3d_uniformcol"]) {
    W::Window *win = windows["3d_uniformcol"];
    win->setPos(wX, wY);
    win->makeCurrentContext();
    rend_3D_UniformCol = new Renderer_3D_1L_UniformCol;
    rend_3D_UniformCol->setUp();
    win->clearCurrentContext();
    NEXT_WINDOW;
  }


  // 3D Uniform Colour, Instanced
  if (tests_enabled["3d_uniformcol_inst"]) {
    W::Window *win = windows["3d_uniformcol_inst"];
    win->setPos(wX, wY);
    win->makeCurrentContext();
    rend_3D_UniformCol_Inst = new Renderer_3D_1L_UniformCol_Instanced;
    rend_3D_UniformCol_Inst->setUp();
    win->clearCurrentContext();
    NEXT_WINDOW;
  }


  // 3D Colour Indexed
  if (tests_enabled["3d_ci"]) {
    NEXT_WINDOW;
    W::Window *win = windows["3d_ci"];
    win->setPos(wX, wY);
    rend_3D_CI = new Renderer_3D_1L_ColourIndexed;
    win->makeCurrentContext();
    rend_3D_CI->setUp();
    win->clearCurrentContext();
  }


  // 3D Colour Indexed, Instanced
  if (tests_enabled["3d_ci_inst"]) {
    W::Window *win = windows["3d_ci_inst"];
    win->setPos(wX, wY);
    win->makeCurrentContext();
    rend_3D_CI_Inst = new Renderer_3D_1L_ColourIndexed_Instanced;
    rend_3D_CI_Inst->setUp();
    win->clearCurrentContext();
    NEXT_WINDOW;
  }


  // 3D Textured
  if (tests_enabled["3d_t"]) {
    W::Window *win = windows["3d_t"];
    win->setPos(wX, wY);
    rend_3D_T = new Renderer_3D_1L_Textured;
    win->makeCurrentContext();
    rend_3D_T->setUp();
    rend_3D_T->setObj(obj_penguin);
    unsigned int tex_penguin = Texture::create();
    Texture::bind(0, tex_penguin);
    Texture::upload(texIm_penguin.w, texIm_penguin.h, texIm_penguin.data, Texture::FilterLinear);
    rend_3D_T->tex = tex_penguin;
    win->clearCurrentContext();
    NEXT_WINDOW;
  }


  // 3D Textured, Instanced
  if (tests_enabled["3d_t_inst"]) {
    W::Window *win = windows["3d_t_inst"];
    win->setPos(wX, wY);
    rend_3D_T_Inst = new Renderer_3D_1L_Textured_Instanced;
    win->makeCurrentContext();
    rend_3D_T_Inst->setUp();
    rend_3D_T_Inst->setObj(obj_penguin);
    unsigned int tex_penguin = Texture::create();
    Texture::bind(0, tex_penguin);
    Texture::upload(texIm_penguin.w, texIm_penguin.h, texIm_penguin.data, Texture::FilterLinear);

    // Upload some test instances
    std::vector<v3> t = { {1.0, 0.0, 0.0}, {-1.0, 0.0, 0.0 } };
    VBO::bind(rend_3D_T_Inst->buffers.translation, VBO::Array);
    VBO::upload(t, VBO::Array, VBO::Static);

    quat q = glm::angleAxis(RAD(33), v3{0.0, 1.0, 0.0});
    v4 qv = { q[0], q[1], q[2], q[3] };
    std::vector<v4> quaternions = { qv, qv };
    VBO::bind(rend_3D_T_Inst->buffers.quaternion, VBO::Array);
    VBO::upload(quaternions, VBO::Array, VBO::Static);

    rend_3D_T_Inst->n_models = 2;

    rend_3D_T_Inst->tex = tex_penguin;
    win->clearCurrentContext();
    NEXT_WINDOW;
  }


  // 3D Reflection Mapped
  if (tests_enabled["3d_rm"]) {
    W::Window *win = windows["3d_rm"];
    win->setPos(wX, wY);
    rend_3D_RM = new Renderer_3D_1L_ReflectionMapped;

    win->makeCurrentContext();
    rend_3D_RM->setUp();
    const char *cubemapFilenames[] = {
      bundledFilePath("Assets/Skybox/right.png"),  bundledFilePath("Assets/Skybox/left.png"),
      bundledFilePath("Assets/Skybox/bottom.png"), bundledFilePath("Assets/Skybox/top.png"),
      bundledFilePath("Assets/Skybox/front.png"),  bundledFilePath("Assets/Skybox/back.png")
    };
    unsigned int rm_tex = Texture::create();
    bool success = loadCubeMap(rm_tex, cubemapFilenames);
    if (!success) {
      printf("Error: couldn't load cube map for Renderer_3D_1L_ReflectionMapped");
    }
    rend_3D_RM->tex = rm_tex;
    win->clearCurrentContext();
    NEXT_WINDOW;
  }


  // Pixel Perfect
  if (tests_enabled["pp"]) {
    W::Window *win = windows["pp"];
    win->setPos(wX, wY);
    win->makeCurrentContext();
    rend_pp = new Renderer_PixelPerfect();
    rend_pp->setUp();

    TexImage texImage = loadPngTexture(bundledFilePath("Assets/cute_heart_icon.png"));
    unsigned int tex_spr = Texture::create();
    Texture::bind(0, tex_spr);
    //    tx_upload(texImage.w, texImage.h, texImage.data, TX_FILTER_LINEAR);

    TexImage texIm2 = get_textTex("Muffins!", 200, 200, 16, {0, 0, 0, 1}, {1, 1, 1, 1});
    Texture::upload(texIm2.w, texIm2.h, texIm2.data, Texture::FilterLinear);

    free(texImage.data);
    free(texIm2.data);

    Renderer_PixelPerfect::Sprite sprHeart = {
      tex_spr,
      { 0, 0, },      // position
      { texIm2.w, texIm2.h }, // native size
      { 0.0, 0.0 },   // position in texture
      { 1.0, 1.0 }    // sprite proportion of texture size
    };
    sprite_heart = rend_pp->add_sprite(sprHeart);
    win->clearCurrentContext();
    NEXT_WINDOW;
  }


  // Pixel Col
  if (tests_enabled["pc"]) {
    W::Window *win = windows["pc"];
    win->setPos(wX, wY);
    win->makeCurrentContext();
    rend_pc = new Renderer_PixelCol;
    rend_pc->setUp();
    rend_pc->setRect({0, 0}, {20, 20});
    rend_pc->setColour({ 0, 0, 0, 0.5 });
    win->clearCurrentContext();
    NEXT_WINDOW;
  }


  // Text
  if (tests_enabled["text"]) {
    W::Window *win = windows["text"];
    win->setPos(wX, wY);
    win->makeCurrentContext();
    rend_text = new Renderer_Text;
    rend_text->setUp();
    win->clearCurrentContext();
    NEXT_WINDOW;
  }


  W::Event::on = true;
  self.gameTimer = [NSTimer timerWithTimeInterval:1./40.
                                           target:self
                                         selector:@selector(timerCB:)
                                         userInfo:nil
                                          repeats:YES];
  [[NSRunLoop mainRunLoop] addTimer:self.gameTimer forMode:NSRunLoopCommonModes];
}

v3 movePositionAlongVector(const v3 &pos, const v3 &vec, float dist) {
  return v3 {
    pos.x + vec.x * dist,
    pos.y + vec.y * dist,
    pos.z + vec.z * dist
  };
}

//#include "FBOToImage.h"

-(void)timerCB:(id)sender {
  for (auto &ev : W::Event::getNewEvents()) {
    if (ev.type == W::EventType::LMouseDown) {
      printf("lmouse down at %d,%d in window %d\n",
             ev.mouseEvent.x,
             ev.mouseEvent.y,
             [self getWindowNum:ev.mouseEvent.window]);
    }
    else if (ev.type == W::EventType::WinClosed) {
      printf("window close event\n");
    }
    else if (ev.type == W::EventType::ScrollWheel) {
      if (ev.scrollEvent.window != windows["3d_uniformcol"]) {
        continue;
      }
    }
    else if (ev.type == W::EventType::KeyDown) {
      printf("Key down!\n");
    }
  }

  float fov = 67;
  float zNear = 0.1;
  float zFar = 100;
  int winw;
  int winh;
  W::Window *first_win = windows.begin()->second;
  first_win->getSize(&winw, &winh);
  cam_3D.setPerspective(fov, winw, winh, zNear, zFar);

  Camera cam_pixel;
  first_win->getSize(&winw, &winh);
  cam_pixel.setPixel(winw, winh);
  m4 mtx_identity = m4(1.0);
  m3 mtx_identity_3d = m3(1.0);

  glDisable(GL_DEPTH_TEST);


  // 2D Colour indexed
  if (tests_enabled["2d_ci"]) {
    W::Window *win = windows["2d_ci"];
    win->makeCurrentContext();
    glViewport(0, 0, winw, winh);
    glClearColor(0.0, 0.75, 0.95, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    rend_2D_CI->render(mtx_identity);
    win->flushBuffer();
    win->clearCurrentContext();
  }


  // 2D Linear Gradient
  if (tests_enabled["lingrad"]) {
    W::Window *win = windows["lingrad"];
    win->makeCurrentContext();
    win->getSize(&winw, &winh);
    glViewport(0, 0, winw, winh);
    glClearColor(0.0, 0.75, 0.95, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    rend_LinGrad->pos = {-1, -1};
    rend_LinGrad->size = {2, 2};
    rend_LinGrad->render(mtx_identity);
    win->flushBuffer();
    win->clearCurrentContext();
  }


  // 2D Textured
  if (tests_enabled["2d_tex"]) {
    W::Window *win = windows["2d_tex"];
    win->makeCurrentContext();
    win->getSize(&winw, &winh);
    glViewport(0, 0, winw, winh);
    glClearColor(0.0, 0.75, 0.95, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    rend_2D_Tex->render(mtx_identity);
    win->flushBuffer();
    win->clearCurrentContext();
  }


  // 3D Uniform Colour
  if (tests_enabled["3d_uniformcol"]) {
    W::Window *win = windows["3d_uniformcol"];
    win->makeCurrentContext();
    glEnable(GL_DEPTH_TEST);
    win->getSize(&winw, &winh);
    glViewport(0, 0, winw, winh);
    glClearColor(0.0, 0.75, 0.95, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    rend_3D_UniformCol->render(cam_3D, light, mtx_identity);
    win->flushBuffer();
    win->clearCurrentContext();
  }


  // 3D Uniform Colour, Instanced
  if (tests_enabled["3d_uniformcol_inst"]) {
    W::Window *win = windows["3d_uniformcol_inst"];
    win->makeCurrentContext();
    glEnable(GL_DEPTH_TEST);
    win->getSize(&winw, &winh);
    glViewport(0, 0, winw, winh);
    glClearColor(0.0, 0.75, 0.95, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    rend_3D_UniformCol_Inst->render(cam_3D, light, mtx_identity_3d);
    win->flushBuffer();
    win->clearCurrentContext();
  }


  // 3D Colour Indexed
  if (tests_enabled["3d_ci"]) {
    W::Window *win = windows["3d_ci"];
    win->makeCurrentContext();
    glEnable(GL_DEPTH_TEST);
    win->getSize(&winw, &winh);
    glViewport(0, 0, winw, winh);
    glClearColor(0.0, 0.75, 0.95, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    rend_3D_CI->render(cam_3D, light, mtx_identity);
    win->flushBuffer();
    win->clearCurrentContext();
  }


  // 3D Colour Indexed, Instanced
  if (tests_enabled["3d_ci_inst"]) {
    W::Window *win = windows["3d_ci_inst"];
    win->makeCurrentContext();
    glEnable(GL_DEPTH_TEST);
    win->getSize(&winw, &winh);
    glViewport(0, 0, winw, winh);
    glClearColor(0.0, 0.75, 0.95, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    rend_3D_CI_Inst->render(cam_3D, light, mtx_identity_3d);
    win->flushBuffer();
    win->clearCurrentContext();
  }


  // 3D Textured
  m4 penguin_rot = glm::rotate(mtx_identity, RAD(180.), v3{0.0, 1.0, 0.0});
  if (tests_enabled["3d_t"]) {
    W::Window *win = windows["3d_t"];
    win->makeCurrentContext();
    glEnable(GL_DEPTH_TEST);
    win->getSize(&winw, &winh);
    glViewport(0, 0, winw, winh);
    glClearColor(0.0, 0.75, 0.95, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    rend_3D_T->render(cam_3D, light, penguin_rot);
    win->flushBuffer();
    win->clearCurrentContext();
  }


  // 3D Textured, Instanced
  if (tests_enabled["3d_t_inst"]) {
    W::Window *win = windows["3d_t_inst"];
    win->makeCurrentContext();
    glEnable(GL_DEPTH_TEST);
    win->getSize(&winw, &winh);
    glViewport(0, 0, winw, winh);
    glClearColor(0.0, 0.75, 0.95, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m3 penguin_rot_3d = m3(penguin_rot);
    rend_3D_T_Inst->render(cam_3D, light, penguin_rot_3d);
    win->flushBuffer();
    win->clearCurrentContext();
  }


  // 3D Reflection Mapped
  if (tests_enabled["3d_rm"]) {
    W::Window *win = windows["3d_rm"];
    win->makeCurrentContext();
    glEnable(GL_DEPTH_TEST);
    win->getSize(&winw, &winh);
    glViewport(0, 0, winw, winh);
    glClearColor(0.0, 0.75, 0.95, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    rend_3D_RM->render(cam_3D, light, mtx_identity);
    win->flushBuffer();
    win->clearCurrentContext();
  }

  // Pixel Perfect
  if (tests_enabled["pp"]) {
    W::Window *win = windows["pp"];
    win->makeCurrentContext();
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    win->getSize(&winw, &winh);
    glViewport(0, 0, winw, winh);
    glClearColor(0.0, 0.75, 0.95, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto &s = rend_pp->get_sprite(sprite_heart);
    s.pos = {0, 0};
    //    (w - s.drawn_size.x) * 0.5f - (int(w)%2 == 0 ? 0.0f : 0.0f),
    //    (h - s.drawn_size.y) * 0.5f - (int(h)%2 == 0 ? 0.0f : 0.0f),
    //  };
    rend_pp->render({(float)winw, (float)winh});
    win->flushBuffer();
    win->clearCurrentContext();
  }

  // Pixel Col
  if (tests_enabled["pc"]) {
    W::Window *win = windows["pc"];
    win->makeCurrentContext();
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    win->getSize(&winw, &winh);
    glViewport(0, 0, winw, winh);
    glClearColor(0.0, 0.75, 0.95, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    rend_pc->setRect({0, 0}, {float(winw/2.), float(winh/2.)});
    rend_pc->render({(float)winw, (float)winh});
    win->flushBuffer();
    win->clearCurrentContext();
  }

  // Text
  if (tests_enabled["text"]) {
    W::Window *win = windows["text"];
    win->makeCurrentContext();
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    win->getSize(&winw, &winh);
    glViewport(0, 0, winw, winh);
    glClearColor(0.0, 0.75, 0.95, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    rend_text->render({(float)winw, (float)winh});
    win->flushBuffer();
    win->clearCurrentContext();
  }

  //  if (i++ == 0) {
  //    CGImageRef imref = newCGImageFromFramebufferContents(0, win_3D_RM->w(), win_3D_RM->h());
  //    CGImageWriteToFile(imref, @"/Users/bh/Desktop/cube.png");
  //    CFRelease(imref);
  //  }
}

-(void)applicationWillTerminate:(NSNotification *)aNotification {
  // Insert code here to tear down your application
}

-(int)getWindowNum:(W::Window*)w {
  return 1;
}

@end

