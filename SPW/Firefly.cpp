#include "Firefly.h"
#include "Scene.h"
#include "Camera.h"
#include "LevelScene.h"


Firefly::Firefly(Scene &scene) :
    Collectable(scene, Layer::COLLECTABLE), m_animator()
{
    m_name = "Firefly";

    RE_Atlas *atlas = scene.GetAssetManager().GetAtlas(AtlasID::COLLECTABLE);
    AssertNew(atlas);

    // Animation "Idle"
    RE_AtlasPart *part = atlas->GetPart("Firefly");
    AssertNew(part);
    RE_TexAnim *idleAnim = new RE_TexAnim(m_animator, "Idle", part);
    idleAnim->SetCycleCount(-1);
    idleAnim->SetCycleTime(0.3f);

    // Couleur des colliders en debug
    m_debugColor.r = 255;
    m_debugColor.g = 0;
    m_debugColor.b = 255;
}

void Firefly::Start()
{
    SetToRespawn(true);

    // Joue l'animation par défaut
    m_animator.PlayAnimation("Idle");

    // Crée le corps
    PE_World &world = m_scene.GetWorld();
    PE_BodyDef bodyDef;
    bodyDef.type = PE_BodyType::DYNAMIC;
    bodyDef.position = GetStartPosition() + PE_Vec2(0.5f, 0.0f);
    bodyDef.name = "Firefly";
    PE_Body *body = world.CreateBody(bodyDef);
    SetBody(body);

    // Crée le collider
    PE_CircleShape circle(PE_Vec2(0.0f, 0.0f), 0.25f);
    PE_ColliderDef colliderDef;
    colliderDef.filter.categoryBits = CATEGORY_COLLECTABLE;
    colliderDef.shape = &circle;
    PE_Collider *collider = body->CreateCollider(colliderDef);
}

void Firefly::Collect(GameBody *collector)
{
    Player *player = dynamic_cast<Player *>(collector);
    if (player == nullptr)
    {
        assert(false);
        return;
    }

    SetEnabled(false);
    player->AddFirefly(1);
}

void Firefly::Render()
{
    SDL_Renderer *renderer = m_scene.GetRenderer();
    Camera *camera = m_scene.GetActiveCamera();

    m_animator.Update(m_scene.GetTime());

    float scale = camera->GetWorldToViewScale();
    SDL_FRect rect = { 0 };
    rect.h = 1.0f * scale;
    rect.w = 1.0f * scale;
    camera->WorldToView(GetPosition(), rect.x, rect.y);
    m_animator.RenderCopyF(&rect, RE_Anchor::CENTER);
}

void Firefly::OnRespawn()
{
    SetBodyEnabled(true);
    SetEnabled(true);
    
    m_animator.StopAnimations();
    m_animator.PlayAnimation("Idle");
}

