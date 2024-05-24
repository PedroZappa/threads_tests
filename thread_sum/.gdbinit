### GDB Settings 
set trace-commands on
set logging enabled on
set print pretty on
set print array on

define main
	display argc
	display *argv@argc
	display data
	display *data
	display *data->idx
	display threads@data.n_threads
	display *list@data.n_numbers
	display g_sum
	display i
	info break
	info threads
end

define routine
	display ref
	display &ref
	display data
	display *data
	display data->idx
	display *data->idx
	display *(list + *data->idx + i)
	display sum
	info threads
end

# ...

# GO GDB GO!

# Start at main
break main
run 2 10
main
fs cmd

# Start at routine
break routine
# run 2 10
# routine
# fs cmd

info break
info threads
