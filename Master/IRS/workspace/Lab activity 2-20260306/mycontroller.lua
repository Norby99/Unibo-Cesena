-- Put your global variables here
MAX_VELOCITY = 15					-- maximum velocity for the robot's wheels
LIGHT_THRESHOLD = 1.5        		-- sum of all light sensors to consider light "detected"
AT_LIGHT_THRESHOLD = 3.7     		-- sum threshold to consider the robot "at" the light source
FORWARD_THRESHOLD = math.pi / 18	-- angle tolerance (rad) to consider "aligned" with light
PROXIMITY_THRESHOLD = 0.1    		-- proximity value above which an obstacle is considered close

FOLLOW_ROTATION_FACTOR = 100.0  -- factor to control how sharply the robot turns when following the light
FEAR_ROTATION_FACTOR = 40.0    -- factor to control how sharply the robot turns when avoiding obstacles

-- States
STATE_SEARCH_LIGHT    = "SEARCH_LIGHT"		-- this state indicates that the robot is searching for the light source
STATE_AVOID_OBSTACLE   = "AVOID_OBSTACLE"	-- this state indicates that the robot is avoiding obstacles
STATE_AT_LIGHT  = "AT_LIGHT"				-- this state indicates that the robot is at the light source

-- Imports
local st = require "states"

n_steps = 0

--[[ This function is executed every time you press the 'execute'
     button ]]
function init()
	robot.wheels.set_velocity(0, 0)
	n_steps = 0
	robot.leds.set_all_colors("black")
	current_state = STATE_SEARCH_LIGHT
	fsm = st.new(robot, MAX_VELOCITY)
end

--[[ Main loop function executed every step ]]
function step()
	n_steps = n_steps + 1
	log("[controller] State: " .. current_state .. " | Step: " .. n_steps)
	update_state()

	if current_state == STATE_SEARCH_LIGHT then
		fsm:follow(robot.light, FOLLOW_ROTATION_FACTOR)
	end

	if current_state == STATE_AVOID_OBSTACLE then
		fsm:fear(robot.proximity, FEAR_ROTATION_FACTOR)
	end

end

--[[ Updates the current state based on sensor readings ]]
function update_state()
	if current_state == STATE_SEARCH_LIGHT and sense_obstacles() then
		log("[controller] Obstacle detected! Switching to AVOID_OBSTACLE state.")
		current_state = STATE_AVOID_OBSTACLE
	end

	if current_state == STATE_AVOID_OBSTACLE and not sense_obstacles() then
		log("[controller] No more obstacles! Switching back to SEARCH_LIGHT state.")
		current_state = STATE_SEARCH_LIGHT
	end

	if sense_light() > AT_LIGHT_THRESHOLD then
		log("[controller] At the light source! Stopping. Light reached in " .. n_steps .. " steps. Switching to AT_LIGHT state.")
		robot.wheels.set_velocity(0, 0)
		current_state = STATE_AT_LIGHT
	end
end

--[[ Senses if there are obstacles nearby using proximity sensors
	 Returns true if an obstacle is detected, false otherwise ]]
function sense_obstacles()
	local obs = false
	for i, sensor in ipairs(robot.proximity) do
		if sensor.value > PROXIMITY_THRESHOLD then
			obs = true
			break
		end
	end
	return obs
end

--[[ Senses the total light intensity from all light sensors
	 Returns the sum of all light sensor values ]]
function sense_light()
	local total_light = 0
	for i, sensor in ipairs(robot.light) do
		total_light = total_light + sensor.value
	end
	return total_light
end

function reset()
	init()
end

--[[ This function is executed only once, when the robot is removed
     from the simulation ]]
function destroy()
   -- put your code here
end
