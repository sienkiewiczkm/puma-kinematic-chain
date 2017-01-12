#include "PumaAnimator.hpp"

namespace application
{

PumaAnimator::PumaAnimator():
    _isPlaying{false},
    _animationElapsed{0.0},
    _animationDuration{1.0}
{
}

PumaAnimator::~PumaAnimator()
{
}

double PumaAnimator::getElapsed() const
{
    return _animationElapsed;
}

double PumaAnimator::getDuration() const
{
    return _animationDuration;
}

void PumaAnimator::setDuration(double animationDuration)
{
    double percentage = _animationElapsed / _animationDuration;
    _animationDuration = animationDuration;
    _animationElapsed = percentage * _animationDuration;
}

void PumaAnimator::setTarget(const std::shared_ptr<PumaCalculator>& target)
{
    _target = target;
}

void PumaAnimator::startAnimation(
    const PumaInverseKinematicsInput& start,
    const PumaInverseKinematicsInput& end,
    const PumaConfiguration& startConfiguration
)
{
    _start = start;
    _end = end;
    _startConfiguration = startConfiguration;
    _animationElapsed = 0.0;

    if (_target != nullptr)
    {
        _target->setConfiguration(_startConfiguration);
    }

    onStart();
}

void PumaAnimator::play()
{
    _isPlaying = true;
}

void PumaAnimator::pause()
{
    _isPlaying = false;
}

void PumaAnimator::restart()
{
    _animationElapsed = 0.0;

    if (_target != nullptr)
    {
        _target->setConfiguration(_startConfiguration);
    }

    onStart();
}

void PumaAnimator::update(double deltaSeconds)
{
    if (_isPlaying)
    {
        _animationElapsed += deltaSeconds;
    }

    if (_animationElapsed > _animationDuration)
    {
        _animationElapsed = _animationDuration;
        _isPlaying = false;
    }

    onStep();
}

}
