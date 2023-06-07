#include "Oneway.h"
#include "Scene.h"
#include "Camera.h"

Oneway::Oneway(Scene &scene) :
    GameBody(scene, Layer::TERRAIN), m_animator()
{
    m_name = "Oneway";
    
    RE_Atlas *atlas = scene.GetAssetManager().GetAtlas(AtlasID::TERRAIN);
    AssertNew(atlas);

    // Animation "Idle"
    RE_AtlasPart *part = atlas->GetPart("OneWay");
    AssertNew(part);
    RE_TexAnim *idleAnim = new RE_TexAnim(m_animator, "Idle", part);
    idleAnim->SetCycleCount(0);

    // Couleur des colliders en debug
    m_debugColor.r = 255;
    m_debugColor.g = 0;
    m_debugColor.b = 255;
}

Oneway::~Oneway()
{
}

void Oneway::Start()
{
    SetToRespawn(true);

    // Joue l'animation par défaut
    m_animator.PlayAnimation("Idle");

    // Crée le corps
PE_World &world = m_scene.GetWorld();
    PE_BodyDef bodyDef;
    bodyDef.type = PE_BodyType::STATIC;
    bodyDef.position = GetStartPosition() + PE_Vec2(0.5f, 0.0f);
    bodyDef.name = "Oneway";
    bodyDef.damping.SetZero();
    PE_Body *body = world.CreateBody(bodyDef);
    SetBody(body);

    // Crée le collider
    PE_ColliderDef colliderDef;
    
    PE_PolygonShape polygon;
    polygon.SetAsBox(PE_AABB(PE_Vec2(-0.5f, 0.0f), PE_Vec2(0.5f, 1.0f)));
    colliderDef.filter.categoryBits = CATEGORY_TERRAIN;
    colliderDef.shape = &polygon;
    colliderDef.isOneWay = true;
    PE_Collider *collider = body->CreateCollider(colliderDef);
}

void Oneway::Render()
{
    SDL_Renderer *renderer = m_scene.GetRenderer();
    Camera *camera = m_scene.GetActiveCamera();

    m_animator.Update(m_scene.GetTime());

    float scale = camera->GetWorldToViewScale();
    SDL_FRect rect = { 0 };
    rect.h = 1.0f * scale;
    rect.w = 1.0f * scale;
    camera->WorldToView(GetPosition(), rect.x, rect.y);
    m_animator.RenderCopyF(&rect, RE_Anchor::SOUTH);
}

void Oneway::OnCollisionEnter(GameCollision &collision)
{
    
}

void Oneway::OnRespawn()
{
    SetBodyEnabled(true);
    SetEnabled(true);
    
    m_animator.StopAnimations();
    m_animator.PlayAnimation("Idle");
}
