# CS321-ProperLaunch
ProperLaunch program for GMU CS321 Summer 2022

# Building the Server
1. you need to be on a linux machiene (or virtual machiene) (debian/ubuntu based preferred, not tested on anything else. so Linux Mint, PopOS, Kubuntu, etc. should work.)
2. clone the repository recursively (IE, add --recursive  to the clone command)
3. download premake https://premake.github.io/ and make sure the binary is on your path (ie, in /usr/bin/)
4. everything else is done from the repo' server directory directory (ie ..../server) on the terminal
5. run build_install_requirements.sh
6. run premake5 gmake2
	* if you see an error message about a script not being run, use chmod to make ALL bash scripts (**.sh) in the `.../server` directory executable.
7. run make -j8
8. if nothing has gone wrong, the server is built
9. run the server (test mode) ./bin/ProperLaunch -t -p testingdata.json
10. if this runs, yay! you are all set. If not, continue to 11.
11. likely, you got a message like :libqlibc.so.1 not found. This means things did not install to the right place.
12. /usr/lib needs to contain several files: 
	- libasyncd.so.1 
	    * (should be a link to same file at /usr/local/lib/ If not present, look in .../server/lib/libasyncd/src for it. Just copy it to /usr/lib)
	- libqlibcext.so
	- libqlibcext.so.2
	- libqlibc.so
	- libqlibc.so.2
	    * (all these should be raw files, if not present, then copy them from ..../server/lib/libasyncd/lib/qlibc/lib)
13. Now it should run. go back to step 9 and try again

## Setting up auto-updates
This is completely unnesecary for running the server. 

To do this, you need to edit the cron tab (`crontab -e`) and add a the below line to have it poll TheSpaceDevs once an hour

`1 * * * * * /path/to/this/repo/localcopy/server/update_database.sh`



# Running the client
1. have nodejs and npm installed
2. from `.../client` run `npm install`
3. Ensure the server is running somewhere. Remember the IP and port.
4. Due to various issues, change the `.../client/src/index.html` file so that script around line 136 has the url variable at the top pointing to the IP and port the server is running at. (it defaults to 127.0.0.1:8888) 
5. run `npm start`

