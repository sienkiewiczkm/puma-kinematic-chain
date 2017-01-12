#include "PumaFastAnimator.hpp"

namespace application
{

PumaFastAnimator::PumaFastAnimator()
{
}

PumaFastAnimator::~PumaFastAnimator()
{
}


void PumaFastAnimator::onStart()
{
    _target->solveIK(
        _start.effectorPosition,
        _start.effectorOrientation
    );

    _startConfiguration = _target->getConfiguration();

    _target->solveIK(
        _end.effectorPosition,
        _end.effectorOrientation
    );

    _endConfiguration = _target->getConfiguration();
}

void PumaFastAnimator::onStep()
{
    double percentage = _animationElapsed / _animationDuration;

    auto mixedConfiguration = PumaConfiguration::mix(
        _startConfiguration,
        _endConfiguration,
        percentage
    );

    _target->setConfiguration(mixedConfiguration);
}

}
