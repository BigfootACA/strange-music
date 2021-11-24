project(
	'strange-music',
	'c',
	version:'0.1',
	default_options:[
		'warning_level=3',
		'strip=true',
		'werror=true',
		'optimization=s',
		'prefix=/usr'
	]
)

executable(
	'strange-music',
	'strange-music.c',
	install:true
)

install_data(
	['strange-music.service','strange-music.timer'],
	install_dir:'lib/systemd/system'
)
