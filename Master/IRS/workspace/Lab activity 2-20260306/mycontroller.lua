-- Put your global variables here
MAX_VELOCITY = 15
LIGHT_THRESHOLD = 1.5        -- sum of all light sensors to consider light "detected"
AT_LIGHT_THRESHOLD = 5.0     -- sum threshold to consider the robot "at" the light source
FORWARD_THRESHOLD = math.pi / 18  -- angle tolerance (rad) to consider "aligned" with light
PROXIMITY_THRESHOLD = 0.1    -- proximity value above which an obstacle is considered close

--[[
TODO:
- clear up the code
- set some variables to global constants
]]

-- States
STATE_SEARCH_LIGHT    = "SEARCH_LIGHT"
STATE_AVOID_OBSTACLE   = "AVOID_OBSTACLE"
STATE_TURN      = "TURN_TO_LIGHT"
STATE_MOVE      = "MOVE_TO_LIGHT"
STATE_AT_LIGHT  = "AT_LIGHT"

-- Imports
local st = require "states"
local db = require "roboto_debug"

n_steps = 0

--[[ This function is executed every time you press the 'execute'
     button ]]
function init()
	robot.wheels.set_velocity(0, 0)
	n_steps = 0
	robot.leds.set_all_colors("black")
	--current_state = STATE_SEARCH_LIGHT
	current_state = STATE_AVOID_OBSTACLE
	fsm = st.new(robot, MAX_VELOCITY)
end

function step()
	n_steps = n_steps + 1
	--log("[controller] State: " .. current_state .. " | Step: " .. n_steps)

	if current_state == STATE_SEARCH_LIGHT then
		fsm:follow(robot.light)
	end

	if current_state == STATE_AVOID_OBSTACLE then
		fsm:fear(robot.proximity)
	end

end

function reset()
	init()
end

--[[ This function is executed only once, when the robot is removed
     from the simulation ]]
function destroy()
   -- put your code here
end
