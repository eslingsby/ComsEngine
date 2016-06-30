-- http://lua-users.org/wiki/InheritanceTutorial

function inheritsFrom(baseClass)
    local new_class = {}
    local class_mt = { __index = new_class }

    -- new_class:create()
    --    local newinst = {}
    --    setmetatable(newinst, class_mt)
    --    return newinst
    --end

    if baseClass then
        setmetatable(new_class, { __index = baseClass })
    end

    return new_class
end

-- Create new table

-- Create metatable

-- Set table to use metatable

-- Set metatable's index to table



-- Create new table

-- Set table to use metatable

-- Store as reference