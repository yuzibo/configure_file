#!/bin/bash

# cron 
# */5 * * * * /home/vimer/monitor/check_tmp.sh

# log
LOG_FILE="/var/log/reboot_trigger.log"

# currently time
CURRENT_TIME=$(date '+%Y-%m-%d %H:%M:%S')

# logical
LOG_TAG="debci"
SINCE="10 minutes ago"
TMPFAIL_COUNT=$(journalctl -t "$LOG_TAG" --since "$SINCE" | grep "tmpfail" | wc -l)

# reboot if tmpfail
if [ "$TMPFAIL_COUNT" -ge 3 ]; then
    echo "[$CURRENT_TIME] Reboot triggered due to $TMPFAIL_COUNT tmpfail(s) detected in the last 5 minutes." >> "$LOG_FILE"

    # close debci-worker 
    echo "[$CURRENT_TIME] Stopping debci-worker service..." >> "$LOG_FILE"
    systemctl stop debci-worker
    if [ $? -eq 0 ]; then
        echo "[$CURRENT_TIME] debci-worker service stopped successfully." >> "$LOG_FILE"
    else
        echo "[$CURRENT_TIME] Failed to stop debci-worker service!" >> "$LOG_FILE"
    fi

    # sleep
    echo "[$CURRENT_TIME] Waiting for 3 minute before reboot..." >> "$LOG_FILE"
    sleep 180

    echo "[$CURRENT_TIME] Initiating system reboot..." >> "$LOG_FILE"

    reboot
else
    echo "[$CURRENT_TIME] No reboot needed. Detected $TMPFAIL_COUNT tmpfail(s) in the last 5 minutes." >> "$LOG_FILE"
fi
