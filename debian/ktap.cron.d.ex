#
# Regular cron jobs for the ktap package
#
0 4	* * *	root	[ -x /usr/bin/ktap_maintenance ] && /usr/bin/ktap_maintenance
