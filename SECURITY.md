Security Policy

Supported Versions
CACA is currently at version 5.1.0, with basic functionality established. Due to its early stage and focus on cryptanalysis, no formal security testing has been conducted. All versions are considered experimental and unsupported for security updates at this time.

Version
Supported
#  ver  #  security #    details     #
|-------|-----------|----------------|
|       |           |                |
| 5.1.0 | :warning: | (Experimental) |
|       |           | (latrest,good) |
| 5.0.1 | :warning: | (Experimental) |
| 5.0.0 | :warning: | (Experimental) |
| 4.0.1 | :warning: | (Experimental) |
| 3.0.0 | :warning: | (Experimental) |
| 2.0.0 | :warning: | (Experimental) |
| 0.1.0 | :warning: | (Experimental) |
| 0.0.1 | :warning: | (Experimental) |
|       |           |                |
#-------#-----------#----------------#

Reporting a Vulnerability

Given CACA's role in cryptanalysis, security is critical but untested. If you identify a vulnerability:

Where to Report: Open an issue on the GitHub repository with the label "security".
Response Time: Expect an update within 7-14 days, though no formal SLA exists yet.
Process: Vulnerabilities will be reviewed by the maintainer. If accepted, a patch will be prioritized; if declined, reasoning will be provided.
Caution: As no security checks have been performed, assume potential risks. Use only in controlled, legal contexts (e.g., security auditing, education) per the project’s ethical intent.


Current Status

Version 5.1.0 is newly operational with zero security validation. Potential vulnerabilities exist due to untested code handling sensitive encryption data. Refer to NIST IR 8446 ipd and Suresh et al. (2018) for guidance on randomness and CA-based analysis risks. Immediate testing and hardening are planned as priorities.
