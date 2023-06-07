#include "Brick.h"
#include "Scene.h"
#include "Camera.h"
#include "Player.h"

Brick::Brick(Scene &scene) :
    GameBody(scene, Layer::TERRAIN), m_animator()
{
    m_name = "Brick";
    
    RE_Atlas *atlas = scene.GetAssetManager().GetAtlas(AtlasID::TERRAIN);
    AssertNew(atlas);

    // Animation "Idle"
    RE_AtlasPart *part = atlas->GetPart("Brick");
    AssertNew(part);
    RE_TexAnim *idleAnim = new RE_TexAnim(m_animator, "Idle", part);
    idleAnim->SetCycleCount(0);

    // Couleur des colliders en debug
    m_debugColor.r = 255;
    m_debugColor.g = 0;
    m_debugColor.b = 255;
}

Brick::~Brick()
{
}

void Brick::Start()
{
    SetToRespawn(false);

    // Joue l'animation par défaut
    m_animator.PlayAnimation("Idle");

    // Crée le corps
    PE_World &world = m_scene.GetWorld();
    PE_BodyDef bodyDef;
    bodyDef.type = PE_BodyType::STATIC;
    bodyDef.position = GetStartPosition() + PE_Vec2(0.5f, 0.0f);
    bodyDef.name = "Brick";
    bodyDef.damping.SetZero();
    PE_Body *body = world.CreateBody(bodyDef);
    SetBody(body);

    // Crée le collider
    PE_ColliderDef colliderDef;
    
    PE_PolygonShape polygon;
    polygon.SetAsBox(PE_AABB(PE_Vec2(-0.5f, 0.0f), PE_Vec2(0.5f, 1.0f)));
    colliderDef.filter.categoryBits = CATEGORY_TERRAIN;
    colliderDef.shape = &polygon;
    PE_Collider *collider = body->CreateCollider(colliderDef);
}

void Brick::Render()
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

void Brick::OnCollisionEnter(GameCollision &collision)
{
    PE_Collider *otherCollider = collision.otherCollider;
    PE_Manifold manifold = collision.manifold;

    // Collision avec le joueur
    if (otherCollider->CheckCategory(CATEGORY_PLAYER))
    {
        Player *player = dynamic_cast<Player *>(collision.gameBody);
        if (player == nullptr)
        {
            assert(false);
            return;
        }

        float angle = PE_AngleDeg(manifold.normal, PE_Vec2::up);
        
        // Si le joueur est sous le bloc, on le casse 
        if (!angle) SetEnabled(false);
    }
}

void Brick::OnRespawn()
{
    SetBodyEnabled(false);
    SetEnabled(false);
    
    m_animator.StopAnimations();
    m_animator.PlayAnimation("Idle");
}
