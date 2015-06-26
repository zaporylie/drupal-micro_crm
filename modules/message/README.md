````drush queue-run crm_message_queue````

This command should be added to crontab. Queue worker limit is 5 minutes so
don't run cron job more often than 5 minutes.
