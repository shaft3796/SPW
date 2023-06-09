#include "GravityModifier.h"
#include "Scene.h"
#include "Camera.h"
#include "LevelScene.h"


GravityModifier::GravityModifier(Scene &scene, float gravity_x, float gravity_y) :
    Collectable(scene, Layer::COLLECTABLE), m_animator(), m_gravity_x(gravity_x), m_gravity_y(gravity_y)
{
    m_name = "GravityModifier";
    

    // Couleur des colliders en debug
    m_debugColor.r = 100;
    m_debugColor.g = 100;
    m_debugColor.b = 255;
}

void GravityModifier::Start()
{
    SetToRespawn(true);
    SetBodyEnabled(false);


    // Crée le corps
    PE_World &world = m_scene.GetWorld();
    PE_BodyDef bodyDef;
    bodyDef.type = PE_BodyType::STATIC;
    bodyDef.position = GetStartPosition() + PE_Vec2(0.5f, 0.0f);
    bodyDef.name = "GravityModifier";
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


void GravityModifier::Render()
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

void GravityModifier::OnRespawn()
{
    SetBodyEnabled(false);
    SetEnabled(true);
}

void GravityModifier::OnCollisionEnter(GameCollision &collision)
{
    PE_Manifold &manifold = collision.manifold;
    PE_Collider *otherCollider = collision.otherCollider;

    // Collision avec le joueur
    if (otherCollider->CheckCategory(CATEGORY_PLAYER))
    {
        Player *player = dynamic_cast<Player *>(collision.gameBody);
        if (player == nullptr)
        {
            assert(false);
            return;
        }

        PE_Body *body = player->GetBody();
        body->SetGravityScale(m_gravity_y);
        

        PE_Vec2 gravity = PE_Vec2(m_gravity_x, m_gravity_y);
        
        m_scene.GetWorld().SetGravity(gravity);
        
    }
}
