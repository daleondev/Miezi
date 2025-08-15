export module mz.graphics.scene.components;

import std;

import mz.graphics.renderer.resources;
import mz.graphics.scene.camera;
import mz.graphics.scene.entity;

import mz.math.geometry;

namespace mz {

    // //------------------------------------------------------
    // //                  Identification
    // //------------------------------------------------------

    // export struct IdComponent
    // {
    //     const UUID id;

    //     IdComponent() : id{uuid()} {}
    //     IdComponent(const IdComponent&) = default;

    //     operator UUID() const { return id; }
    // };

    // export struct TagComponent
    // {
    //     std::string tag;

    //     TagComponent() = default;
    //     TagComponent(const TagComponent&) = default;
    //     TagComponent(const std::string& tag) : tag{ tag } {}

    //     operator std::string() const { return tag; }
    // };

    // //------------------------------------------------------
    // //                    Transform
    // //------------------------------------------------------

    // export struct TransformComponent
    // {
    //     Vec3 translation;
    //     glm::quat rotation;
    //     Vec3 scale;

    //     struct DetectChangeData
    //     {
    //         EdgeDetector<Vec3> translationListener;
    //         EdgeDetector<glm::quat> rotationListener;
    //         EdgeDetector<Vec3> scaleListener;
    //         std::optional<std::function<void(const Mat4&)>> changedCallback;
    //     } detectChangeData;

    //     TransformComponent() : translation{ 0.0f }, rotation{ Mat3(1.0f) }, scale{ 1.0f } {};
    //     TransformComponent(const TransformComponent&) = default;

    //     operator Mat4() const { return Mat4(1.0f)
    //         .translated(translation)
    //         .rotated(rotation)
    //         .scaled(scale); }
    // };

    // //------------------------------------------------------
    // //                       Renderer
    // //------------------------------------------------------

    // export struct PointRendererComponent
	// {
	// 	Vec4 color;
    //     float size;

	// 	PointRendererComponent() : color{ 1.0f }, size{ 1.0f } {}
	// 	PointRendererComponent(const PointRendererComponent&) = default;
	// 	PointRendererComponent(const Vec4& color, const float size = 1.0f) : color{ color }, size{ size } {}
	// };

    // export struct LineRendererComponent
	// {
	// 	Vec4 color;
    //     float thickness;
    //     bool smooth;

	// 	LineRendererComponent() : color{ 1.0f }, thickness{ 1.0f }, smooth{ false } {}
	// 	LineRendererComponent(const LineRendererComponent&) = default;
	// 	LineRendererComponent(const Vec4& color, const float thickness = 1.0f, const bool smooth = false) : color{ color }, thickness{ thickness }, smooth{ smooth } {}
	// };

    // export struct RectRendererComponent
	// {
    //     std::variant<Vec4, std::shared_ptr<ITexture>> material;

	// 	RectRendererComponent() : material{ Vec4(1.0f) } {}
	// 	RectRendererComponent(const RectRendererComponent&) = default;
	// 	RectRendererComponent(const Vec4& color) : material{ color } {}
	// 	RectRendererComponent(const std::shared_ptr<ITexture>& texture) : material{ texture } {}
	// };

    // export struct CircleRendererComponent
	// {
    //     std::variant<Vec4, std::shared_ptr<ITexture>> material;

	// 	CircleRendererComponent() : material{ Vec4(1.0f) } {}
	// 	CircleRendererComponent(const CircleRendererComponent&) = default;
	// 	CircleRendererComponent(const Vec4& color) : material{ color } {}
	// 	CircleRendererComponent(const std::shared_ptr<ITexture>& texture) : material{ texture } {}
	// };

    // export struct PlaneRendererComponent
	// {
    //     std::variant<Vec4, std::shared_ptr<ITexture>> material;

	// 	PlaneRendererComponent() : material{ Vec4(1.0f) } {}
	// 	PlaneRendererComponent(const PlaneRendererComponent&) = default;
	// 	PlaneRendererComponent(const Vec4& color) : material{ color } {}
	// 	PlaneRendererComponent(const std::shared_ptr<ITexture>& texture) : material{ texture } {}
	// };

    // export struct BoxRendererComponent
	// {
    //     std::variant<Vec4, std::shared_ptr<ITexture>> material;

	// 	BoxRendererComponent() : material{ Vec4(1.0f) } {}
	// 	BoxRendererComponent(const BoxRendererComponent&) = default;
	// 	BoxRendererComponent(const Vec4& color) : material{ color } {}
	// 	BoxRendererComponent(const std::shared_ptr<ITexture>& texture) : material{ texture } {}
	// };

    // export struct SphereRendererComponent
	// {
    //     std::variant<Vec4, std::shared_ptr<ITexture>> material;

	// 	SphereRendererComponent() : material{ Vec4(1.0f) } {}
	// 	SphereRendererComponent(const SphereRendererComponent&) = default;
	// 	SphereRendererComponent(const Vec4& color) : material{ color } {}
	// 	SphereRendererComponent(const std::shared_ptr<ITexture>& texture) : material{ texture } {}
	// };

    // export struct MeshRendererComponent
	// {
    //     std::string meshName;
    //     std::variant<Vec4, std::shared_ptr<ITexture>> material;

	// 	MeshRendererComponent(const std::string& name) : meshName{ name }, material{ Vec4(1.0f) } {}
	// 	MeshRendererComponent(const MeshRendererComponent&) = default;
	// 	MeshRendererComponent(const std::string& name, const Vec4& color) : meshName{ name }, material{ color } {}
	// 	MeshRendererComponent(const std::string& name, const std::shared_ptr<ITexture>& texture) : meshName{ name }, material{ texture } {}
	// };

    // export struct PointcloudRendererComponent
	// {
    //     std::size_t pcId;
    //     std::shared_ptr<ITexture> texture;

	// 	PointcloudRendererComponent(const std::size_t id, const std::shared_ptr<ITexture>& texture) : pcId{ id }, texture{ texture } {}
	// 	PointcloudRendererComponent(const PointcloudRendererComponent&) = default;
	// };

    // //------------------------------------------------------
    // //                      Properties
    // //------------------------------------------------------

    // export struct CameraComponent
	// {
	// 	SceneCamera Camera;

	// 	CameraComponent() = default;
	// 	CameraComponent(const CameraComponent&) = default;
	// };

    // //------------------------------------------------------
    // //                   Component Group
    // //------------------------------------------------------

    // export template<typename... Component>
	// struct ComponentGroup
	// {
	// };

    // export using AllComponents = 
	// 	ComponentGroup<TransformComponent, CameraComponent>;

}