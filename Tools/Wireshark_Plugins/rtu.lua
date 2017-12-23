
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
    pktlen = pktlen - 3
    
    -- We start by adding our protocol to the dissection display tree.
    -- A call to tree:add() returns the child created, so we can add more "under" it using that return value.
    -- The second argument is how much of the buffer/packet this added tree item covers/represents - in this
    -- case (DNS protocol) that's the remainder of the packet.
    
    local command = tvbuf(1, pktlen):string()
    local payload = tonumber(command)
    local payload_cmd = bit.band(command, 7)

    local payload_addr = bit.rshift(bit.band(command, 2040), 3)
    
    tree:add("Command: " .. payload_cmd)
    tree:add("Address: " .. payload_addr)

    -- Now let's add our transaction id under our dns protocol tree we just created.
    -- The transaction id starts at offset 0, for 2 bytes length.
    
end
    
----------------------------------------
-- we want to have our protocol dissection invoked for a specific UDP port,
-- so get the udp dissector table and add our protocol to it
DissectorTable.get("udp.port"):add(default_settings.port, robo)   