
local debug_level = {
    DISABLED = 0,
    LEVEL_1  = 1,
    LEVEL_2  = 2
}

-- set this DEBUG to debug_level.LEVEL_1 to enable printing debug_level info
-- set it to debug_level.LEVEL_2 to enable really verbose printing
-- note: this will be overridden by user's preference settings
local DEBUG = debug_level.LEVEL_1

local default_settings =
{
    debug_level  = DEBUG,
    port         = 2017,
    heur_enabled = false,
}

local robo = Proto("myrobo","Falcon Protocol")

local ef_too_short = ProtoExpert.new("myrobo.too_short.expert", "DNS message too short",
                                     expert.group.MALFORMED, expert.severity.ERROR)

local pf_command            = ProtoField.uint16("myrobo.command", "Command")
local pf_address            = ProtoField.uint16("myrobo.address", "Address")
local pf_data               = ProtoField.int32("myrobo.data", "Data")

local PKT_LEN = 3

----------------------------------------
-- The following creates the callback function for the dissector.
-- It's the same as doing "dns.dissector = function (tvbuf,pkt,root)"
-- The 'tvbuf' is a Tvb object, 'pktinfo' is a Pinfo object, and 'root' is a TreeItem object.
-- Whenever Wireshark dissects a packet that our Proto is hooked into, it will call
-- this function and pass it these arguments for the packet it's dissecting.

function robo.dissector(tvbuf,pktinfo,root)

    pktinfo.cols.protocol:set("FALCON") -- set the protocol column to show our protocol name
    
    -- We want to check that the packet size is rational during dissection, so let's get the length of the packet buffer (Tvb).
    local pktlen = tvbuf:reported_length_remaining()
    local tree = root:add(robo, tvbuf:range(0,pktlen))
    local type = tvbuf(0,1):string()
    
    pktlen = pktlen - 3
    
    -- We start by adding our protocol to the dissection display tree.
    -- A call to tree:add() returns the child created, so we can add more "under" it using that return value.
    
    local raw_command = tvbuf(1, pktlen):string()
    local items = split(raw_command,',') -- split with ','
    local command_type = 0

    tree:add("Raw Command: " .. raw_command)

    if type == '{' then
        command_type = 1 -- request
    elseif type == '[' then
        command_type = 2 -- response
    else
        command_type = 0 -- invalid
    end
    
    for key, value in pairs(items) do

        local subtree = tree:add("Element " .. "Raw: " .. value, tvbuf:range(0,pktlen))
        -- subtree:add("Single command: " .. value)
        local command = tonumber(value)
        local payload_cmd = bit.band(command, 7)

        if command_type == 1 then
            local payload_addr = bit.rshift(bit.band(command, 2040), 3)

            if payload_cmd == 1 then -- For read command
                subtree:add("Command: " .. payload_cmd .. " [Read]")
    
            elseif payload_cmd == 2 then -- For write command
                subtree:add("Command: " .. payload_cmd .. " [Write]")
                local payload_data = bit.rshift(bit.band(command, 134215680),11)
    
                local payload_data_sign = bit.rshift(bit.band(command, 134217728),27)
                if payload_data_sign == 1 then -- signed number
                    payload_data = 0 - payload_data
                end
                subtree:add("Data: " .. payload_data)
    
            else -- For invalid command
                subtree:add("Command: " .. payload_cmd .. " [Invalid Command]")
            end
        
            subtree:add("Address: " .. payload_addr)

        elseif command_type == 2 then
            local data = command
            subtree:add("Data: " .. data)
        end
    end

end

function split(s, delimiter)
    result = {};
    for match in (s..delimiter):gmatch("(.-)"..delimiter) do
        table.insert(result, match);
    end
    return result;
end

DissectorTable.get("udp.port"):add(default_settings.port, robo)   