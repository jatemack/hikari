#include "hikari/core/game/StateTransition.hpp"
#include "hikari/client/game/FadeColorTask.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace hikari {

    class FadeStateTransition : public StateTransition {
    public:
        enum FadeDirection {
            FADE_OUT = 0,
            FADE_IN = 1
        };

    private:
        FadeDirection direction;
        sf::RectangleShape overlay;
        FadeColorTask fadeTask;

    public:
        FadeStateTransition(FadeDirection direction, sf::Color color, float duration);
        virtual ~FadeStateTransition();

        virtual void render(sf::RenderTarget &target);
        virtual void update(float dt);
    };

} // hikari
