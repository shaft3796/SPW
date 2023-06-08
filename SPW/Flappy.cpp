#include "Flappy.h"
#include "Scene.h"
#include "Camera.h"
#include "LevelScene.h"
#include "Graphics.h"

Flappy::Flappy(Scene &scene) :
    Enemy(scene), m_animator(), m_state(State::IDLE), m_speed(FLAPPY_SPEED), m_distance(FLAPPY_DISTANCE)
{
    m_name = "Flappy";

    RE_Atlas *atlas = scene.GetAssetManager().GetAtlas(AtlasID::ENEMY);
    AssertNew(atlas);

    // Animation "Idle"
    RE_AtlasPart *part = atlas->GetPart("FlappyIdle");
    AssertNew(part);
    RE_TexAnim *idleAnim = new RE_TexAnim(m_animator, "Idle", part);
    idleAnim->SetCycleCount(0);
}

Flappy::~Flappy()
{
}

void Flappy::Start()
{
    SetToRespawn(true);

    // Joue l'animation par d�faut
    m_animator.PlayAnimation("Idle");

    // Cr�e le corps
    PE_World &world = m_scene.GetWorld();
    PE_BodyDef bodyDef;
    bodyDef.type = PE_BodyType::STATIC;
    bodyDef.position = GetStartPosition() + PE_Vec2(0.5f, 0.0f);
    bodyDef.name = "Flappy";
    bodyDef.damping.SetZero();
    PE_Body *body = world.CreateBody(bodyDef);
    SetBody(body);

    // Cr�e le collider
    PE_CircleShape circle(PE_Vec2(0.05f, 0.8f), 0.4f);
    PE_ColliderDef colliderDef;
    colliderDef.friction = 0.005f;
    colliderDef.filter.categoryBits = CATEGORY_TERRAIN | CATEGORY_ENEMY;
    colliderDef.shape = &circle;
    PE_Collider *collider = body->CreateCollider(colliderDef);

    // Endort le corps
    // Permet d'optimiser le calcul de la physique,
    // seuls les corps proches du joueur sont simul�s
    body->SetAwake(false);
}

void Flappy::FixedUpdate()
{
    PE_Body *body = GetBody();
    PE_Vec2 position = body->GetPosition();

    position.y = cos(m_scene.GetTime().GetElapsed()*m_speed) * m_distance + GetStartPosition().y;
    
    if (body->IsAwake() == false)
    {
        // Ne met pas � jour l'ennemi s'il est endormi
        // Le joueur est loin de lui et il est plus visible par la cam�ra.
        return;
    }

    LevelScene *levelScene = dynamic_cast<LevelScene *>(&m_scene);
    if (levelScene == nullptr)
    {
        assert(false);
        return;
    }

    Player *player = levelScene->GetPlayer();

    float dist = PE_Distance(position, player->GetPosition());

    if (dist > 24.0f)
    {
        // La distance entre de joueur et l'ennemi vient de d�passer 24 tuiles.
        // On endort l'ennemi pour ne plus la simuler dans le moteur physique.
        body->SetAwake(false);
        return;
    }

    body->SetPosition(position);

}

void Flappy::Render()
{
    SDL_Renderer *renderer = m_scene.GetRenderer();
    Camera *camera = m_scene.GetActiveCamera();

    m_animator.Update(m_scene.GetTime());

    SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;

    float scale = camera->GetWorldToViewScale();
    SDL_FRect rect = { 0 };
    rect.h = 1.3f * scale;
    rect.w = 1.3f * scale;
    camera->WorldToView(GetPosition(), rect.x, rect.y);
    m_animator.RenderCopyExF(&rect, RE_Anchor::SOUTH, 0, Vec2(0.f, 0.f), flip);
}

void Flappy::OnRespawn()
{
    m_state = State::IDLE;

    SetToRespawn(true);
    SetBodyEnabled(true);
    SetEnabled(true);

    PE_Body *body = GetBody();
    body->SetPosition(GetStartPosition());
    body->SetVelocity(PE_Vec2::zero);
    body->ClearForces();

    m_animator.StopAnimations();
    m_animator.PlayAnimation("Idle");
}

void Flappy::Damage(GameBody *damager)
{
    Player *player = dynamic_cast<Player *>(damager);
    if (player == nullptr)
    {
        assert(false);
        return;
    }
    player->Bounce();
    SetEnabled(false);
}

void Flappy::OnCollisionStay(GameCollision &collision)
{
    PE_Manifold &manifold = collision.manifold;
    PE_Collider *otherCollider = collision.otherCollider;

    if (m_state == State::DYING){
        collision.SetEnabled(false);
        return;
    }

    // Collision avec le joueur
    if (otherCollider->CheckCategory(CATEGORY_PLAYER))
    {
        Player *player = dynamic_cast<Player *>(collision.gameBody);
        if (player == nullptr)
        {
            assert(false);
            return;
        }
        float angle = PE_SignedAngleDeg(manifold.normal, PE_Vec2::down);
        if (fabsf(angle) > PLAYER_DAMAGE_ANGLE)
        {
            player->Damage();
        }
        return;
    }
}
