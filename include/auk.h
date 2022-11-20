#pragma once 

#include <SDL.h>

#include "auk/platform/window.h"
#include "auk/platform/log.h"
#include "auk/platform/file.h"
#include "auk/platform/time.h"
#include "auk/platform/type_id.h"

#include "auk/graphics/graphics.h"
#include "auk/graphics/shader.h"
#include "auk/graphics/texture.h"
#include "auk/graphics/renderer.h"
#include "auk/graphics/target.h"
#include "auk/graphics/color.h"

#include "auk/numerics/matrix.h"
#include "auk/numerics/linalg.h"
#include "auk/numerics/mathf.h"
#include "auk/numerics/rect.h"

#include "auk/input/keys.h"
#include "auk/input/input.h"
#include "auk/input/virtualaxis.h"
#include "auk/input/virtualbutton.h"
#include "auk/input/virtualinput.h"

#include "auk/util/state_machine.h"
#include "auk/util/svector.h"
#include "auk/util/type_table.h"
#include "auk/util/virtual_map.h"