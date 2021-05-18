#ifndef GUMIGOTA_WATER_TILE_H
#define GUMIGOTA_WATER_TILE_H



/* INCLUDES */
// Header Includes
#include "../../../maths/vectors/vector3.h"



namespace gg
{

	class WaterTile
	{
	protected:
		/* Variables */
		vector3 m_position;
		float m_rotation; //stored in radians
		float m_scale;

		float m_movementSpeed;
		float m_movementFactor;
		float m_tilingFactor;
		float m_waveStrength;
		float m_lightShineDamper;
		float m_lightReflectivity;

	public:
		/* Constructors */
		WaterTile(const vector3& pos);
		WaterTile(const vector3& pos, const float scale);

		/* Functions */
		void update(const float deltaTime);

		// Setters
		void move(const vector3& movement);
		void rotate(const float rot);
		void scale(const float scale);

		void setPosition(const vector3& pos);
		void setRotation(const float rot);
		void setScale(const float scale);

		void setMovementSpeed(const float speed);
		void setMovementFactor(const float value);
		void addMovementFactor(const float value);
		void setTilingFactor(const float value);
		void setWaveStrength(const float value);
		void setLightShineDamper(const float value);
		void setLightReflectivity(const float value);

		// Getters
		inline const vector3& getPosition() const { return m_position; }
		inline       float    getRotation() const { return m_rotation; }
		inline       float    getScale()    const { return m_scale;    }

		inline float getMovementSpeed()       const { return m_movementSpeed;     }
		inline float getMovementFactor()      const { return m_movementFactor;    }
		inline float getTilingFactor()        const { return m_tilingFactor;      }
		inline float getWaveStrength()        const { return m_waveStrength;      }
		inline float getLightShineDamper()    const { return m_lightShineDamper;  }
		inline float getLightReflectivity()   const { return m_lightReflectivity; }
	};

}



#endif