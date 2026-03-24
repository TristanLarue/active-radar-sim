#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Functions.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/Math/Vector2.h>
#include <Magnum/Math/Vector3.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Primitives/UVSphere.h>
#include <Magnum/Shaders/Phong.h>
#include <Magnum/Trade/MeshData.h>

#include "headers/Simulation.hpp"

namespace RadarSim {
namespace {

Magnum::Color3 ColorFromViewId(unsigned int viewId) {
    const float seed = static_cast<float>(viewId % 360u);
    const float r = 0.35f + 0.65f * Magnum::Math::abs(Magnum::Math::sin(Magnum::Deg(seed)));
    const float g = 0.35f + 0.65f * Magnum::Math::abs(Magnum::Math::sin(Magnum::Deg(seed + 120.0f)));
    const float b = 0.35f + 0.65f * Magnum::Math::abs(Magnum::Math::sin(Magnum::Deg(seed + 240.0f)));
    return {r, g, b};
}

} // namespace

class VizualisationApp final : public Magnum::Platform::Application {
public:
    explicit VizualisationApp(const Arguments& arguments);

private:
    void drawEvent() override;
    void viewportEvent(ViewportEvent& event) override;
    void keyPressEvent(KeyEvent& event) override;
    void keyReleaseEvent(KeyEvent& event) override;
    Magnum::Matrix4 BuildViewMatrix() const;
    void UpdateCamera(double deltaTimeSeconds);

    Simulation _simulation;
    Magnum::GL::Mesh _sphere;
    Magnum::Shaders::Phong _shader;
    Magnum::Matrix4 _projection;
    Magnum::Vector3 _cameraPosition{0.0f, 1.5f, 3.0f};
    float _cameraYawDeg = 180.0f;
    float _cameraPitchDeg = -10.0f;

    bool _moveForward = false;
    bool _moveBackward = false;
    bool _moveLeft = false;
    bool _moveRight = false;
    bool _moveUp = false;
    bool _moveDown = false;
    bool _turnLeft = false;
    bool _turnRight = false;
    bool _lookUp = false;
    bool _lookDown = false;
};

VizualisationApp::VizualisationApp(const Arguments& arguments)
    : Magnum::Platform::Application{arguments, Configuration{}
          .setTitle("Active Radar Sim Baseline")
          .setSize({1280, 720})}
    , _sphere{Magnum::MeshTools::compile(Magnum::Primitives::uvSphereSolid(16, 32))} {
    using namespace Magnum;

    GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
    GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);

    _projection = Matrix4::perspectiveProjection(Deg{35.0f}, Vector2{windowSize()}.aspectRatio(), 0.01f, 100.0f);

    _shader
        .setAmbientColor(Color3{0.066f})
        .setDiffuseColor(Color3{0.431f, 0.643f, 1.0f})
        .setSpecularColor(Color3{1.0f})
        .setShininess(80.0f)
        .setLightPositions({{5.0f, 5.0f, 7.0f}});
}

void VizualisationApp::drawEvent() {
    using namespace Magnum;
    constexpr double deltaTimeSeconds = 1.0 / 60.0;

    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);

    _simulation.Tick(deltaTimeSeconds);
    UpdateCamera(deltaTimeSeconds);
    const Matrix4 view = BuildViewMatrix();

    // Debug reference rendered in world space with camera transform applied.
    {
        const Matrix4 model = Matrix4::translation({0.0f, 0.0f, -4.0f});
        const Matrix4 modelView = view * model;
        _shader
            .setDiffuseColor(Color3{0.35f, 1.0f, 0.35f})
            .setTransformationMatrix(modelView)
            .setNormalMatrix(modelView.normalMatrix())
            .setProjectionMatrix(_projection)
            .draw(_sphere);
    }

    for(const EntitySnapshot& snapshot : _simulation.GetSnapshots()) {
        const Vector3 position{
            snapshot.position[0],
            snapshot.position[1],
            snapshot.position[2]
        };
        const Vector3 orientationDeg{
            snapshot.orientationEulerDeg[0],
            snapshot.orientationEulerDeg[1],
            snapshot.orientationEulerDeg[2]
        };

        const Matrix4 model =
            Matrix4::translation(position) *
            Matrix4::rotationX(Deg{orientationDeg.x()}) *
            Matrix4::rotationY(Deg{orientationDeg.y()}) *
            Matrix4::rotationZ(Deg{orientationDeg.z()});
        const Matrix4 modelView = view * model;

        _shader
            .setDiffuseColor(ColorFromViewId(snapshot.viewId))
            .setTransformationMatrix(modelView)
            .setNormalMatrix(modelView.normalMatrix())
            .setProjectionMatrix(_projection)
            .draw(_sphere);
    }

    swapBuffers();
    redraw();
}

void VizualisationApp::viewportEvent(ViewportEvent& event) {
    using namespace Magnum;

    GL::defaultFramebuffer.setViewport({{}, event.framebufferSize()});
    _projection = Matrix4::perspectiveProjection(Deg{35.0f}, Vector2{event.windowSize()}.aspectRatio(), 0.01f, 100.0f);
}

void VizualisationApp::keyPressEvent(KeyEvent& event) {
    using Key = KeyEvent::Key;

    if(event.key() == Key::W) _moveForward = true;
    else if(event.key() == Key::S) _moveBackward = true;
    else if(event.key() == Key::A) _moveLeft = true;
    else if(event.key() == Key::D) _moveRight = true;
    else if(event.key() == Key::E) _moveUp = true;
    else if(event.key() == Key::Q) _moveDown = true;
    else if(event.key() == Key::Left) _turnLeft = true;
    else if(event.key() == Key::Right) _turnRight = true;
    else if(event.key() == Key::Up) _lookUp = true;
    else if(event.key() == Key::Down) _lookDown = true;
    else return;

    event.setAccepted();
}

void VizualisationApp::keyReleaseEvent(KeyEvent& event) {
    using Key = KeyEvent::Key;

    if(event.key() == Key::W) _moveForward = false;
    else if(event.key() == Key::S) _moveBackward = false;
    else if(event.key() == Key::A) _moveLeft = false;
    else if(event.key() == Key::D) _moveRight = false;
    else if(event.key() == Key::E) _moveUp = false;
    else if(event.key() == Key::Q) _moveDown = false;
    else if(event.key() == Key::Left) _turnLeft = false;
    else if(event.key() == Key::Right) _turnRight = false;
    else if(event.key() == Key::Up) _lookUp = false;
    else if(event.key() == Key::Down) _lookDown = false;
    else return;

    event.setAccepted();
}

Magnum::Matrix4 VizualisationApp::BuildViewMatrix() const {
    using namespace Magnum;

    const Vector3 forward{
        Math::cos(Deg{_cameraPitchDeg}) * Math::sin(Deg{_cameraYawDeg}),
        Math::sin(Deg{_cameraPitchDeg}),
        Math::cos(Deg{_cameraPitchDeg}) * Math::cos(Deg{_cameraYawDeg})
    };

    // Magnum's camera convention expects the camera to look towards -Z by default.
    // If input yaw/pitch ends up inverted (common across conventions), flipping the forward
    // direction makes the scene visible without changing the rest of the pipeline.
    const Vector3 target = _cameraPosition - forward;
    const Vector3 worldUp{0.0f, 1.0f, 0.0f};
    return Matrix4::lookAt(_cameraPosition, target, worldUp);
}

void VizualisationApp::UpdateCamera(double deltaTimeSeconds) {
    using namespace Magnum;

    const float moveSpeed = 8.0f;
    const float turnSpeedDeg = 90.0f;
    const float dt = static_cast<float>(deltaTimeSeconds);

    if(_turnLeft) _cameraYawDeg += turnSpeedDeg * dt;
    if(_turnRight) _cameraYawDeg -= turnSpeedDeg * dt;
    if(_lookUp) _cameraPitchDeg += turnSpeedDeg * dt;
    if(_lookDown) _cameraPitchDeg -= turnSpeedDeg * dt;

    if(_cameraPitchDeg > 85.0f) _cameraPitchDeg = 85.0f;
    if(_cameraPitchDeg < -85.0f) _cameraPitchDeg = -85.0f;

    const Vector3 forward{
        Math::cos(Deg{_cameraPitchDeg}) * Math::sin(Deg{_cameraYawDeg}),
        Math::sin(Deg{_cameraPitchDeg}),
        Math::cos(Deg{_cameraPitchDeg}) * Math::cos(Deg{_cameraYawDeg})
    };
    const Vector3 worldUp{0.0f, 1.0f, 0.0f};
    const Vector3 forwardDir = -forward;
    const Vector3 right = Math::cross(forwardDir, worldUp).normalized();

    if(_moveForward) _cameraPosition += forwardDir.normalized() * moveSpeed * dt;
    if(_moveBackward) _cameraPosition -= forwardDir.normalized() * moveSpeed * dt;
    if(_moveRight) _cameraPosition += right * moveSpeed * dt;
    if(_moveLeft) _cameraPosition -= right * moveSpeed * dt;
    if(_moveUp) _cameraPosition += worldUp * moveSpeed * dt;
    if(_moveDown) _cameraPosition -= worldUp * moveSpeed * dt;
}

} // namespace RadarSim

int RunVizualisation(int argc, char** argv) {
    RadarSim::VizualisationApp app({argc, argv});
    return app.exec();
}
