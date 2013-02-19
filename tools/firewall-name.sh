#!/bin/bash
#####################
#
# firewall-name.sh
#
# reports name of current firewall using the qiptables convention
# of using a chain of zero references whose name is prefixed with
# "Q_" to match the shortname column of the ruleset table.
#
#####################

ZERO_REFERENCES="(0 references)"
Q_IPTABLES_PREFIX="Q_"

iptables -L | grep "$Q_IPTABLES_PREFIX" | awk '{ print $2; }'



