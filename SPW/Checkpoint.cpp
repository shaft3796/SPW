#include "Checkpoint.h"
#include "Scene.h"
#include "Camera.h"
#include "LevelScene.h"

Checkpoint::Checkpoint(Scene &scene) :
    GameBody(scene, Layer::TERRAIN_BACKGROUND), m_animator(), m_isActive(true)
{
    m_name = "Checkpoint";
    
    AssetManager &assetsManager = scene.GetAssetManager();
    RE_Atlas *atlas = assetsManager.GetAtlas(AtlasID::LEVER);
    RE_AtlasPart *part = nullptr;

    // Animation "lever on"
    part = atlas->GetPart("LeverOn");
    AssertNew(part);
    RE_TexAnim *idleAnim = new RE_TexAnim(
        m_animator, "LeverOn", part
    );
    idleAnim->SetCycleCount(0);

    // Animation "lever off"
    part = atlas->GetPart("LeverOff");
    AssertNew(part);
    RE_TexAnim *fallingAnim = new RE_TexAnim(
            m_animator, "LeverOff", part
    );
    fallingAnim->SetCycleCount(0);

    // Couleur des colliders en debug
    m_debugColor.r = 255;
    m_debugColor.g = 100;
    m_debugColor.b = 100;
}

void Checkpoint::Start()
{
    SetToRespawn(true);

    // Joue l'animation par défaut
    m_animator.PlayAnimation("LeverOff");

    // Crée le corps
    PE_World &world = m_scene.GetWorld();
    PE_BodyDef bodyDef;
    bodyDef.type = PE_BodyType::STATIC;
    bodyDef.position = GetStartPosition() + PE_Vec2(0.5f, 0.0f);
    bodyDef.name = "Checkpoint";
    PE_Body *body = world.CreateBody(bodyDef);
    SetBody(body);

    // Crée le collider
    PE_CircleShape circle(PE_Vec2(0.0f, -0.07f), 0.3f);
    PE_ColliderDef colliderDef;
    colliderDef.friction = 0.0f;
    colliderDef.filter.categoryBits = CATEGORY_TERRAIN;
    colliderDef.shape = &circle;
    PE_Collider *collider = body->CreateCollider(colliderDef);
}

void Checkpoint::Render()
{
    SDL_Renderer *renderer = m_scene.GetRenderer();
    Camera *camera = m_scene.GetActiveCamera();

    m_animator.Update(m_scene.GetTime());

    float scale = camera->GetWorldToViewScale();
    SDL_FRect rect = { 0 };
    float coef = 1.5;
    rect.h = 0.40f * scale * coef;
    rect.w = 1.330f * scale * coef;
    camera->WorldToView(GetPosition(), rect.x, rect.y);
    m_animator.RenderCopyF(&rect, RE_Anchor::SOUTH);
}

void Checkpoint::OnCollisionEnter(GameCollision &collision)
{
    if (collision.otherCollider->CheckCategory(CATEGORY_PLAYER))
    {
        if (!m_isActive) return;
        m_animator.PlayAnimation("LeverOn");
        LevelScene *levelScene = dynamic_cast<LevelScene *>(&m_scene);
        if (levelScene == nullptr)
        {
            assert(false);
            return;
        }

        Player *player = levelScene->GetPlayer();
        PE_Vec2 position = collision.gameBody->GetPosition();
        position.x -= 1.0f;
        player->SetStartPosition(position);
        levelScene->SetResetTimer(false);
        m_isActive = false;
    }
}
