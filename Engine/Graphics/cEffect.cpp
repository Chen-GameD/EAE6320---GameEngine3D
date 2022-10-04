// Includes
//=========

#include "cEffect.h"

#include <Engine/ScopeGuard/cScopeGuard.h>
#include <Engine/Logging/Logging.h>

eae6320::cResult eae6320::Graphics::cEffect::CreateEffect(cEffect*& o_effect, const char* i_vertexShaderAddress, const char* i_fragmentShaderAddress)
{
	auto result = Results::Success;

	cEffect* newEffect = nullptr;
	cScopeGuard scopeGuard([&o_effect, &result, &newEffect]
		{
			if (result)
			{
				EAE6320_ASSERT(newEffect != nullptr);
				o_effect = newEffect;
			}
			else
			{
				if (newEffect)
				{
					newEffect->DecrementReferenceCount();
					newEffect = nullptr;
				}
				o_effect = nullptr;
			}
		});

	// Allocate a new effect
	{
		newEffect = new (std::nothrow) cEffect();
		if (!newEffect)
		{
			result = Results::OutOfMemory;
			EAE6320_ASSERTF(false, "Couldn't allocate memory for the effect");
			Logging::OutputError("Failed to allocate memory for the effect");
			return result;
		}
	}

	// Initialize the shading data
	if (!(result = newEffect->InitializeShadingData(i_vertexShaderAddress, i_fragmentShaderAddress)))
	{
		EAE6320_ASSERTF(false, "Can't initialize Graphics without the shading data");
		return result;
	}

	return result;
}

eae6320::Graphics::cEffect::~cEffect()
{
	EAE6320_ASSERT(m_referenceCount == 0);
	const auto result = CleanUp();
	EAE6320_ASSERT(result);
}