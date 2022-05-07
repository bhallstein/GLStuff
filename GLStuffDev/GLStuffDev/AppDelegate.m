#import "AppDelegate.h"
#import "OGLW/W.h"
#import <OpenGL/gl.h>

#import "Camera.h"
#import "Lights.h"

#include "glm_include.h"
#include "CoordinateTypes.h"
#include "Primitives.h"

#include "Renderer_2D_ColourIndexed.h"
#include "Renderer_LinearGradient.h"
#include "Renderer_2D_Textured.h"
#include "Renderer_3D_1L_UniformCol.h"
#include "Renderer_3D_1L_UniformCol_Instanced.h"
#include "Renderer_3D_1L_ColourIndexed.h"
#include "Renderer_3D_1L_ColourIndexed_Instanced.h"
#include "Renderer_3D_1L_Textured.h"
#include "Renderer_3D_1L_Textured_Instanced.h"
#include "Renderer_3D_1L_ReflectionMapped.h"
#include "Renderer_PixelPerfect.h"
#include "Renderer_PixelCol.h"
#include "Renderer_Text.h"

#include "FilePaths_CPP.h"
#include "TexImage.h"
#include "ObjImport.h"

#include "GLProg.h"
#include "TextTex.h"

#define PROBABILITY(x) \
if (rand() / (float)RAND_MAX < x)
#define OTHERWISE \
else

@interface AppDelegate () {
  W::Window *win_2DCI;
  W::Window *win_LinGrad;
  W::Window *win_2DTex;

  W::Window *win_3DUniformCol;
  W::Window *win_3DUniformCol_Inst;
  W::Window *win_3DCI;
  W::Window *win_3DCI_Inst;
  W::Window *win_3D_T;
  W::Window *win_3D_T_Inst;
  W::Window *win_3D_RM;
  W::Window *win_pp;
  W::Window *win_pc;
  W::Window *win_text;

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
  Renderer_PixelCol *rend_pc;
  Renderer_Text *rend_text;

  DirectionalLight light;
  Camera cam_3D;

  unsigned int spr_heart;

  int i;
    //  float counter;
}
@property NSTimer *gameTimer;

@end


@implementation AppDelegate

-(IBAction) setR:(NSSlider*)sender { /*rend_3D_1L_UniformCol->setR(sender.floatValue);*/ }
-(IBAction) setG:(NSSlider*)sender { /*rend_3D_1L_UniformCol->setG(sender.floatValue);*/ }
-(IBAction) setB:(NSSlider*)sender { /*rend_3D_1L_UniformCol->setB(sender.floatValue);*/ }
-(IBAction) setRatio:(NSSlider*)sender { /*rend_3D_1L_UniformCol->setRatio(sender.floatValue);*/ }

float getCamAngleForHeight(float y) {
  float minAngle    = 40.0, maxAngle = 70.0;
  float angle_scale = (y - 1.0) / 9.0;
  float new_angle   = (maxAngle - minAngle) * angle_scale;
  return -new_angle;
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {

    // Init cam
  float initial_cam_height = 2.5;
  cam_3D.setPosition({0.0, initial_cam_height, 6.25});
  cam_3D.setOrientation(getCamAngleForHeight(initial_cam_height), {1.0, 0, 0});

    // Set up light
  float one_over_root_3 = 0.577;
  light.lightVector[0] = -one_over_root_3;
  light.lightVector[1] = -one_over_root_3;
  light.lightVector[2] = -one_over_root_3;
  light.lightProperties[0] = 0.5;
  light.lightProperties[1] = 0.6;
  light.lightProperties[2] = 0.6;

    //  float frustW = windowW / pixelsToWorldUnits;
    //  float frustH = windowH / pixelsToWorldUnits;
    //  cam.setOrthographic(-frustW*0.5, frustW*0.5, -frustH*0.5, frustH*0.5, 0.01, 1000.0);

  float defaultWinW = 300.0;
  float defaultWinH = 240.0;

    // Window & renderer setup

  int scrW, scrH;
  W::getScreenSize(0, &scrW, &scrH);
  W::Multisampling::T msLevel = W::Multisampling::X4;

  int wX = 0, wY = 23;

#define NEXT_WIN do { \
if (wX + 2*defaultWinW > scrW) wX = 0, wY += defaultWinH + 22; \
else wX += defaultWinW; \
} while(0)

    // 2D Colour Indexed
  {
    win_2DCI = new W::Window(defaultWinW,
                             defaultWinH,
                             "2D Colour Indexed",
                             0,
                             false,
                             0,
                             msLevel);
    win_2DCI->setPos(wX, wY);
    win_2DCI->makeCurrentContext();
    rend_2D_CI = new Renderer_2D_ColourIndexed(true);
    rend_2D_CI->setUp();

    TexImage texImage = get_textTex("Muffins", 200, 100, 40, {0, 0, 0, 1}, {1, 1, 1, 1});
    unsigned int tex = tx_create();
    tx_bind(tex);
    tx_upload(texImage.w, texImage.h, texImage.data, TX_FILTER_LINEAR);
    free(texImage.data);

    win_2DCI->clearCurrentContext();
  }

    // Linear Gradient
  {
    NEXT_WIN;
    win_LinGrad = new W::Window(defaultWinW,
                                defaultWinH,
                                "Linear Gradient",
                                0, false, 0, msLevel);
    win_LinGrad->setPos(wX, wY);
    win_LinGrad->makeCurrentContext();
    rend_LinGrad = new Renderer_LinearGradient(true);
    rend_LinGrad->setUp();
    rend_LinGrad->setColours({ 5./255, 117./255, 137./255, 1.0 },
                             { 118./255, 221./255, 225./255, 1.0 });
    win_LinGrad->clearCurrentContext();
  }

    // 2D Textured
  {
    NEXT_WIN;
    win_2DTex = new W::Window(defaultWinW,
                              defaultWinH,
                              "2D Textured",
                              0,
                              false,
                              0,
                              msLevel);
    win_2DTex->setPos(wX, wY);
    win_2DTex->makeCurrentContext();
    rend_2D_Tex = new Renderer_2D_Textured;

    TexImage texImage = loadPngTexture(bundledFilePath("Shiny-bubble.png"));
    unsigned int tex = tx_create();
    tx_bind(tex);
    tx_upload(texImage.w, texImage.h, texImage.data, TX_FILTER_LINEAR);
    free(texImage.data);

    rend_2D_Tex->setUp();
    rend_2D_Tex->setTex(tex);

    Rectangle2D r = unitSquare * 2.0;
    vbo_bind(rend_2D_Tex->buffers.vertexPos, VBOTYPE_ARRAY);
    vbo_upload(sizeof(v2)*6, r.vertices, VBOTYPE_ARRAY, VBOHINT_STATIC);

    Rectangle2D t = unitSquare_texcoords;
    vbo_bind(rend_2D_Tex->buffers.texCoord, VBOTYPE_ARRAY);
    vbo_upload(sizeof(v2)*6, t.vertices, VBOTYPE_ARRAY, VBOHINT_STATIC);

    rend_2D_Tex->n_vertices = 6;

    win_2DTex->clearCurrentContext();
  }



    // 3D Uniform Colour
  {
    NEXT_WIN;
    win_3DUniformCol = new W::Window(defaultWinW,
                                     defaultWinH,
                                     "3D Uniform Colour",
                                     0,
                                     false,
                                     0,
                                     msLevel);
    win_3DUniformCol->setPos(wX, wY);
    win_3DUniformCol->makeCurrentContext();
    rend_3D_UniformCol = new Renderer_3D_1L_UniformCol;
    rend_3D_UniformCol->setUp();
    win_3DUniformCol->clearCurrentContext();
  }

    // 3D Uniform Colour, Instanced
  {
    NEXT_WIN;
    win_3DUniformCol_Inst = new W::Window(defaultWinW,
                                          defaultWinH,
                                          "3D Uniform Colour - Instanced",
                                          0,
                                          false,
                                          0,
                                          msLevel);
    win_3DUniformCol_Inst->setPos(wX, wY);
    win_3DUniformCol_Inst->makeCurrentContext();
    rend_3D_UniformCol_Inst = new Renderer_3D_1L_UniformCol_Instanced;
    rend_3D_UniformCol_Inst->setUp();
    win_3DUniformCol_Inst->clearCurrentContext();
  }

    // 3D Colour Indexed
  {
    NEXT_WIN;
    win_3DCI = new W::Window(defaultWinW,
                             defaultWinH,
                             "3D Colour Indexed",
                             0,
                             false,
                             0,
                             msLevel);
    win_3DCI->setPos(wX, wY);
    rend_3D_CI = new Renderer_3D_1L_ColourIndexed;
    win_3DCI->makeCurrentContext();
    rend_3D_CI->setUp();
    win_3DCI->clearCurrentContext();
  }

    // 3D Colour Indexed, Instanced
  {
    NEXT_WIN;
    win_3DCI_Inst = new W::Window(defaultWinW,
                                  defaultWinH,
                                  "3D Colour Indexed - Instanced",
                                  0,
                                  false,
                                  0,
                                  msLevel);
    win_3DCI_Inst->setPos(wX, wY);
    win_3DCI_Inst->makeCurrentContext();
    rend_3D_CI_Inst = new Renderer_3D_1L_ColourIndexed_Instanced;
    rend_3D_CI_Inst->setUp();
    win_3DCI_Inst->clearCurrentContext();
  }

    // Load penguin obj
  ObjFile obj_penguin = loadObjFile(bundledFilePath("Penguin/Penguin.obj"));
  printObjFileSummary(&obj_penguin, "Penguin.obj");

    // Load penguin tex
  TexImage texIm_penguin = loadPngTexture(bundledFilePath("Penguin/penguin_box.png"));

    // 3D Textured
  {
    NEXT_WIN;
    win_3D_T = new W::Window(defaultWinW,
                             defaultWinH,
                             "3D Textured",
                             0,
                             false,
                             0,
                             msLevel);
    win_3D_T->setPos(wX, wY);
    rend_3D_T = new Renderer_3D_1L_Textured;
    win_3D_T->makeCurrentContext();
    rend_3D_T->setUp();
    rend_3D_T->setObj(&obj_penguin);
    unsigned int tex_penguin = tx_create();
    tx_bind(tex_penguin);
    tx_upload(texIm_penguin.w, texIm_penguin.h, texIm_penguin.data, TX_FILTER_LINEAR);
    rend_3D_T->setTex(tex_penguin);
    win_3D_T->clearCurrentContext();
  }

    // 3D Textured, Instanced
  {
    NEXT_WIN;
    win_3D_T_Inst = new W::Window(defaultWinW,
                                  defaultWinH,
                                  "3D Textured - Instanced",
                                  0,
                                  false,
                                  0,
                                  msLevel);
    win_3D_T_Inst->setPos(wX, wY);
    rend_3D_T_Inst = new Renderer_3D_1L_Textured_Instanced;
    win_3D_T_Inst->makeCurrentContext();
    rend_3D_T_Inst->setUp();
    rend_3D_T_Inst->setObj(&obj_penguin);
    unsigned int tex_penguin = tx_create();
    tx_bind(tex_penguin);
    tx_upload(texIm_penguin.w, texIm_penguin.h, texIm_penguin.data, TX_FILTER_LINEAR);

      // Upload some test instances
    v3 t[] = { {1.0, 0.0, 0.0}, {-1.0, 0.0, 0.0 } };
    vbo_bind(rend_3D_T_Inst->buffers.translation, VBOTYPE_ARRAY);
    vbo_upload(sizeof(v3)*2, t, VBOTYPE_ARRAY, VBOHINT_STATIC);

    glm::quat q = glm::angleAxis(RAD(33), glm::vec3(0.0, 1.0, 0.0));
    v4 qv = { q[0], q[1], q[2], q[3] };
    v4 qs[] = { qv, qv };
    vbo_bind(rend_3D_T_Inst->buffers.quaternion, VBOTYPE_ARRAY);
    vbo_upload(sizeof(v4)*2, qs, VBOTYPE_ARRAY, VBOHINT_STATIC);

    rend_3D_T_Inst->n_models = 2;

    rend_3D_T_Inst->setTex(tex_penguin);
    win_3D_T_Inst->clearCurrentContext();
  }

    // 3D Reflection Mapped
  {
    NEXT_WIN;
    win_3D_RM = new W::Window(defaultWinW,
                              defaultWinH,
                              "3D Reflection Mapped",
                              0,
                              false,
                              0,
                              msLevel);
    win_3D_RM->setPos(wX, wY);
    rend_3D_RM = new Renderer_3D_1L_ReflectionMapped;

    win_3D_RM->makeCurrentContext();
    rend_3D_RM->setUp();
    const char *cubemapFilenames[] = {
      bundledFilePath("A_Skybox/right.png"),  bundledFilePath("A_Skybox/left.png"),
      bundledFilePath("A_Skybox/bottom.png"), bundledFilePath("A_Skybox/top.png"),
      bundledFilePath("A_Skybox/front.png"),  bundledFilePath("A_Skybox/back.png")
    };
    unsigned int rm_tex = tx_create();
    bool success = loadCubeMap(rm_tex, cubemapFilenames);
    if (!success) {
      printf("Error: couldn't load cube map for Renderer_3D_1L_ReflectionMapped");
    }
    rend_3D_RM->setTex(rm_tex);
    win_3D_RM->clearCurrentContext();
  }

    // Pixel Perfect
  {
    NEXT_WIN;
    win_pp = new W::Window(defaultWinW,
                           defaultWinH,
                           "Pixel Perfect",
                           0,
                           false,
                           0,
                           msLevel);
    win_pp->setPos(wX, wY);
    win_pp->makeCurrentContext();
    rend_pp = new Renderer_PixelPerfect();
    rend_pp->setUp();

    TexImage texImage = loadPngTexture(bundledFilePath("cute_heart_icon.png"));
    unsigned int tex_spr = tx_create();
    tx_bind(tex_spr);
      //    tx_upload(texImage.w, texImage.h, texImage.data, TX_FILTER_LINEAR);

    TexImage texIm2 = get_textTex("Muffins!", 200, 200, 16, {0, 0, 0, 1}, {1, 1, 1, 1});
    tx_upload(texIm2.w, texIm2.h, texIm2.data, TX_FILTER_LINEAR);

    free(texImage.data);
    free(texIm2.data);

    Renderer_PixelPerfect::Sprite sprHeart = {
      tex_spr,
      { 0, 0, },      // position
      { texIm2.w, texIm2.h }, // native size
      { 0.0, 0.0 },   // position in texture
      { 1.0, 1.0 }    // sprite proportion of texture size
    };
    spr_heart = rend_pp->addSprite(sprHeart);
    win_pp->clearCurrentContext();
  }

    // Pixel Col
  {
    NEXT_WIN;
    win_pc = new W::Window(defaultWinW,
                           defaultWinH,
                           "Pixel Col",
                           0,
                           false,
                           0,
                           msLevel);
    win_pc->setPos(wX, wY);
    win_pc->makeCurrentContext();
    rend_pc = new Renderer_PixelCol;
    rend_pc->setUp();
    rend_pc->setRect({0, 0}, {20, 20});
    rend_pc->setColour({ 0, 0, 0, 0.5 });
    win_pc->clearCurrentContext();
  }

    // Text
  {
    NEXT_WIN;
    win_text = new W::Window(defaultWinW,
                             defaultWinH,
                             "Text",
                             0,
                             false,
                             0,
                             msLevel);
    win_text->setPos(wX, wY);
    win_text->makeCurrentContext();
    rend_text = new Renderer_Text;
    rend_text->setUp();
    win_text->clearCurrentContext();
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
    if (ev.type == W::EventType::LMouseDown)
      printf("lmouse down at %d,%d in window %d\n",
             ev.mouseEvent.x,
             ev.mouseEvent.y,
             [self getWindowNum:ev.mouseEvent.window]);
    else if (ev.type == W::EventType::WinClosed)
      printf("window close event\n");
    else if (ev.type == W::EventType::ScrollWheel) {
      if (ev.scrollEvent.window != win_3DUniformCol)
        continue;

      float dy = ev.scrollEvent.dy;
      v3 campos = { cam_3D.pos[0], cam_3D.pos[1], cam_3D.pos[2] };
      v3 new_campos = movePositionAlongVector(campos, {0,1,1}, dy * 0.2);

      float new_angle = getCamAngleForHeight(new_campos.y);
      cam_3D.setOrientation(new_angle, {1.0, 0.0, 0.0});

      if (new_campos.y >= 0.2 && new_campos.y <= 12.0)
        cam_3D.setPosition(new_campos);
    }
    else if (ev.type == W::EventType::KeyDown)
      printf("Key down!\n");
  }

  float fov = 67;
  float zNear = 0.1;
  float zFar = 100;
  int winw, winh;
  win_3DUniformCol->getSize(&winw, &winh);
  cam_3D.setPerspective(fov, winw, winh, zNear, zFar);

  Camera cam_pixel;
  win_2DCI->getSize(&winw, &winh);
  cam_pixel.setPixel(winw, winh);
  glm::mat4 mtx_identity = glm::mat4(1.0);
  glm::mat3 mtx_identity_3d = glm::mat3(1.0);

  glDisable(GL_DEPTH_TEST);


    // 2D Colour indexed
  win_2DCI->makeCurrentContext();
  glViewport(0, 0, winw, winh);
  glClearColor(0.0, 0.75, 0.95, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  rend_2D_CI->render(mptr(mtx_identity));
  win_2DCI->flushBuffer();
  win_2DCI->clearCurrentContext();


    // 2D Linear Gradient
  win_LinGrad->makeCurrentContext();
  win_LinGrad->getSize(&winw, &winh);
  glViewport(0, 0, winw, winh);
  glClearColor(0.0, 0.75, 0.95, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  rend_LinGrad->setPos(-1, -1, 2, 2);
  rend_LinGrad->render(mptr(mtx_identity));
  win_LinGrad->flushBuffer();
  win_LinGrad->clearCurrentContext();


    // 2D Textured
  win_2DTex->makeCurrentContext();
  win_2DTex->getSize(&winw, &winh);
  glViewport(0, 0, winw, winh);
  glClearColor(0.0, 0.75, 0.95, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  rend_2D_Tex->render(mptr(mtx_identity));
  win_2DTex->flushBuffer();
  win_2DTex->clearCurrentContext();


    // 3D Uniform Colour
  win_3DUniformCol->makeCurrentContext();
  glEnable(GL_DEPTH_TEST);
  win_3DUniformCol->getSize(&winw, &winh);
  glViewport(0, 0, winw, winh);
  glClearColor(0.0, 0.75, 0.95, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  rend_3D_UniformCol->render(&cam_3D, &light, mtx_identity);
  win_3DUniformCol->flushBuffer();
  win_3DUniformCol->clearCurrentContext();


    // 3D Uniform Colour, Instanced
  win_3DUniformCol_Inst->makeCurrentContext();
  glEnable(GL_DEPTH_TEST);
  win_3DUniformCol_Inst->getSize(&winw, &winh);
  glViewport(0, 0, winw, winh);
  glClearColor(0.0, 0.75, 0.95, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  rend_3D_UniformCol_Inst->render(&cam_3D, &light, mtx_identity_3d);
  win_3DUniformCol_Inst->flushBuffer();
  win_3DUniformCol_Inst->clearCurrentContext();


    // 3D Colour Indexed
  win_3DCI->makeCurrentContext();
  glEnable(GL_DEPTH_TEST);
  win_3DCI->getSize(&winw, &winh);
  glViewport(0, 0, winw, winh);
  glClearColor(0.0, 0.75, 0.95, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  rend_3D_CI->render(&cam_3D, &light, mtx_identity);
  win_3DCI->flushBuffer();
  win_3DCI->clearCurrentContext();


    // 3D Colour Indexed, Instanced
  win_3DCI_Inst->makeCurrentContext();
  glEnable(GL_DEPTH_TEST);
  win_3DCI_Inst->getSize(&winw, &winh);
  glViewport(0, 0, winw, winh);
  glClearColor(0.0, 0.75, 0.95, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  rend_3D_CI_Inst->render(&cam_3D, &light, mtx_identity_3d);
  win_3DCI_Inst->flushBuffer();
  win_3DCI_Inst->clearCurrentContext();


    // 3D Textured
  win_3D_T->makeCurrentContext();
  glEnable(GL_DEPTH_TEST);
  win_3D_T->getSize(&winw, &winh);
  glViewport(0, 0, winw, winh);
  glClearColor(0.0, 0.75, 0.95, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glm::mat4 penguin_rot = glm::rotate(mtx_identity, RAD(180.), glm::vec3(0.0, 1.0, 0.0));
  rend_3D_T->render(&cam_3D, &light, penguin_rot);
  win_3D_T->flushBuffer();
  win_3D_T->clearCurrentContext();


    // 3D Textured, Instanced
  win_3D_T_Inst->makeCurrentContext();
  glEnable(GL_DEPTH_TEST);
  win_3D_T_Inst->getSize(&winw, &winh);
  glViewport(0, 0, winw, winh);
  glClearColor(0.0, 0.75, 0.95, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glm::mat3 penguin_rot_3d = glm::mat3(penguin_rot);
  rend_3D_T_Inst->render(&cam_3D, &light, penguin_rot_3d);
  win_3D_T_Inst->flushBuffer();
  win_3D_T_Inst->clearCurrentContext();


    // 3D Reflection Mapped
  win_3D_RM->makeCurrentContext();
  glEnable(GL_DEPTH_TEST);
  win_3D_RM->getSize(&winw, &winh);
  glViewport(0, 0, winw, winh);
  glClearColor(0.0, 0.75, 0.95, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  rend_3D_RM->render(&cam_3D, &light, mtx_identity);
  win_3D_RM->flushBuffer();
  win_3D_RM->clearCurrentContext();

    // Pixel Perfect
  win_pp->makeCurrentContext();
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  win_pp->getSize(&winw, &winh);
  glViewport(0, 0, winw, winh);
  glClearColor(0.0, 0.75, 0.95, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  auto &s = rend_pp->getSprite(spr_heart);
  s.pos = {0, 0};
    //    (w - s.drawn_size.x) * 0.5f - (int(w)%2 == 0 ? 0.0f : 0.0f),
    //    (h - s.drawn_size.y) * 0.5f - (int(h)%2 == 0 ? 0.0f : 0.0f),
    //  };
  rend_pp->render({(float)winw, (float)winh});
  win_pp->flushBuffer();
  win_pp->clearCurrentContext();

    // Pixel Col
  win_pc->makeCurrentContext();
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  win_pc->getSize(&winw, &winh);
  glViewport(0, 0, winw, winh);
  glClearColor(0.0, 0.75, 0.95, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  rend_pc->setRect({0, 0}, {float(winw/2.), float(winh/2.)});
  rend_pc->render({(float)winw, (float)winh});
  win_pc->flushBuffer();
  win_pc->clearCurrentContext();

    // Text
  win_text->makeCurrentContext();
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  win_pc->getSize(&winw, &winh);
  glViewport(0, 0, winw, winh);
  glClearColor(0.0, 0.75, 0.95, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  rend_text->render({(float)winw, (float)winh});
  win_text->flushBuffer();
  win_text->clearCurrentContext();

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

