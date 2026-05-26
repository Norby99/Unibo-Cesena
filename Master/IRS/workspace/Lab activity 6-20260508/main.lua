-- Put your global variables here
MAX_VELOCITY = 15	-- The maximum velocity of the robot, in pixels per second

--[[ The maximum values perceived by the robot's sensors, used for normalizing the
sensor values. These values can be adjusted based on the specific characteristics of
the robot's sensors and the environment it operates in.
]]
MAX_LIGHT_PERCIVED = 0.3	
MAX_PROXIMITY_PERCIVED = 1.0
MIN_MOTOR_GROUND_PERCIVED = 0.01

RANDOM_WANDER_TICKS = 2

local WeightsManager = require("libraries.weights_manager")

local weights_manager = WeightsManager.new({
	fear = 0.5,
	random_wander = 0.5
})
DEFAULT_WEIGHT = 1.0

WEIGHTS = weights_manager:get_weights()

PARAMS = {
	W = 0.1,
	S = 0.01,
	Psmax = 0.99,
	Pwmin = 0.005,
	a = 0.1,
	b = 0.05,

	N = 0,
	MAXRANGE = 30.0,

	Dw = 0.1,
	Ds = 0.01
}

-- Imports
local MotorSchemaController = require("libraries.motor_schema_controller")
local Follow = require("libraries.behaviors.follow")
local Fear = require("libraries.behaviors.fear")
local StopAtDarkSpot = require("libraries.behaviors.stop_at_dark_spot")
local RandomWander = require("libraries.behaviors.random_wander")

local db = require("libraries.roboto_debug")

n_steps = 0

-- Setup function, called once at the beginning of the simulation
function init()
	robot.wheels.set_velocity(0, 0)
	n_steps = 0
	robot.leds.set_all_colors("black")

	controller = MotorSchemaController.new(MAX_VELOCITY, WEIGHTS)

	--local stopAtDarkSpot = StopAtDarkSpot.new(MAX_VELOCITY, robot.motor_ground, MIN_MOTOR_GROUND_PERCIVED)
	local fear = Fear.new(MAX_VELOCITY, robot.proximity, MAX_PROXIMITY_PERCIVED)
	local randomWander = RandomWander.new(MAX_VELOCITY, robot.proximity, RANDOM_WANDER_TICKS)

	controller:add(fear)
    controller:add(randomWander)
end

-- Step function, called at each simulation step
function step()
	n_steps = n_steps + 1

	local left_vel, right_vel = controller:run()

	robot.wheels.set_velocity(left_vel, right_vel)
end

function reset()
	init()
end

--[[ This function is executed only once, when the robot is removed
     from the simulation ]]
function destroy()
   robot.wheels.set_velocity(0, 0)
end
