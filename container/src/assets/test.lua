local box={}
for i=1,32 do
	table.insert(box,{x=10*i,vel=0})
end
local gravity=0.05
local boost=60
local speed=0.01

local player={
	x=200,y=0,vel=0
}

function xavine.update(dt)
	for i=1,#box do
		box[i].x=box[i].x+box[i].vel*dt*speed
		box[i].vel=box[i].vel-gravity*dt
		if box[i].x<=0 then
			box[i].x=0
			box[i].vel=boost
		end
	end
end

function xavine.draw()
	for i=1,#box do
		xavine.set_color(i/#box,1-i/#box,0,1)
		xavine.draw_rect(15*i,490-box[i].x,10,10)
	end
	xavine.set_color(0,0,1,0.8)
	xavine.draw_rect(player.x,485-player.y,15,15)
end

function xavine.keypressed(key)
	print('luakey: ',key)
	if key=='Right'then
		player.x=player.x+15
	elseif key=='Left'then
		player.x=player.x-15
	end
end
