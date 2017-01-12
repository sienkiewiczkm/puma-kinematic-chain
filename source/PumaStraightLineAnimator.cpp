#include "PumaStraightLineAnimator.hpp"

namespace application
{

PumaStraightLineAnimator::PumaStraightLineAnimator()
{
}

PumaStraightLineAnimator::~PumaStraightLineAnimator()
{
}

void PumaStraightLineAnimator::onStart()
{
}

void PumaStraightLineAnimator::onStep()
{
    double percentage = _animationElapsed / _animationDuration;

    auto interpolatedPosition = glm::mix(
        _start.effectorPosition,
        _end.effectorPosition,
        static_cast<float>(percentage)
    );

    auto interpolatedQuat = glm::normalize(
        glm::slerp(
            _start.effectorOrientation,
            _end.effectorOrientation,
            static_cast<float>(percentage)
        )
    );

    _target->solveIK(interpolatedPosition, interpolatedQuat);
}

}
