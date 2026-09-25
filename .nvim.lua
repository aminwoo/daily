-- Loaded by ./daily open (and by nvim itself if you enable `vim.o.exrc = true`).
if vim.g.daily_loaded then return end
vim.g.daily_loaded = true

local root = vim.fn.fnamemodify(debug.getinfo(1, 'S').source:sub(2), ':p:h')
local daily = root .. '/daily'

-- :make  → compile + run tests, compiler errors land in the quickfix list
vim.o.makeprg = daily .. ' test %'

-- <leader>t → save everything and run the tests in a terminal split below
vim.keymap.set('n', '<leader>t', function()
  vim.cmd('wall')
  local file = vim.fn.expand('%:p')
  vim.cmd('botright 18split')
  vim.cmd('terminal ' .. daily .. ' test ' .. vim.fn.shellescape(file))
  vim.cmd('startinsert')
end, { desc = 'daily: run tests' })

-- <leader>r → save everything and run your sample cases (work/<ex>.sample.cpp)
vim.keymap.set('n', '<leader>r', function()
  vim.cmd('wall')
  local file = vim.fn.expand('%:p')
  vim.cmd('botright 18split')
  vim.cmd('terminal ' .. daily .. ' sample ' .. vim.fn.shellescape(file))
  vim.cmd('startinsert')
end, { desc = 'daily: run sample cases' })

-- <leader>m → same as <leader>t, but through :make so errors are jumpable with :cn / :cp
vim.keymap.set('n', '<leader>m', ':wall | make<CR>', { desc = 'daily: make' })

-- :DailyNext / :DailyOpen <ex> → switch both panes to another exercise in place
local function open_exercise(args)
  local out = vim.fn.systemlist(daily .. ' ' .. args)
  local ex = out[#out]
  if vim.v.shell_error ~= 0 or not ex or ex == '' then
    vim.notify(table.concat(out, '\n'), vim.log.levels.WARN)
    return
  end
  vim.cmd('wall')
  vim.cmd('silent! only')
  -- close the previous exercise's buffers (work file, README, test terminal)
  local old = vim.api.nvim_list_bufs()
  vim.cmd('edit ' .. vim.fn.fnameescape(root .. '/work/' .. ex .. '.cpp'))
  vim.cmd('vsplit ' .. vim.fn.fnameescape(root .. '/exercises/' .. ex .. '/README.md'))
  vim.cmd('wincmd h')
  for _, b in ipairs(old) do
    if vim.api.nvim_buf_is_valid(b) and vim.fn.bufwinnr(b) == -1 then
      pcall(vim.api.nvim_buf_delete, b, { force = true })
    end
  end
  vim.notify('daily: ' .. ex)
end

vim.api.nvim_create_user_command('DailyNext', function() open_exercise('next-id') end,
  { desc = 'daily: open the next unpassed exercise' })
vim.api.nvim_create_user_command('DailyOpen', function(o) open_exercise('resolve ' .. vim.fn.shellescape(o.args)) end,
  { nargs = 1, desc = 'daily: open an exercise by number/name' })

-- <leader>n → next unpassed exercise (same as :DailyNext)
vim.keymap.set('n', '<leader>n', function() open_exercise('next-id') end, { desc = 'daily: next exercise' })
