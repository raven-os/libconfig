# Communication lib-service {#Communication}

*Note: if you're a client side application, you most likelly want to use the library, which is documented in the [library header](@ref Albinos.h)*

## Requests

All requests must contain **REQUEST_NAME**, containing the type of action they want to do.
Each response contain at least **REQUEST_STATE** (see below).
All the local settings are applied after the config inclusions.

### Request types
| Request Name | Request Description | Additional Argument(s) | Additional Returned Value(s) | Config ref count change |
| -------- | -------- | -------- | -------- | -------- |
|*CONFIG_CREATE*|Create a new config.|**CONFIG_NAME**|**CONFIG_KEY**<br>**READONLY_CONFIG_KEY**| +1 |
|*CONFIG_LOAD*| Load an existing config. |**CONFIG_KEY** *or* **READONLY_CONFIG_KEY**| **CONFIG_NAME**<br>**CONFIG_ID**| +1 |
|*CONFIG_UNLOAD*| Unload config. |**CONFIG_ID**|*none*| -1 |
|*CONFIG_DESTROY*| Destroy config. |**CONFIG_ID**|*none*| -1 |
|*CONFIG_GET_DEPS*| Get the ordered list of all included configs. |**CONFIG_ID**|**DEPS** (array of CONFIG_ID giving read-only access to each dependency)| +1 for deps |
|*CONFIG_GET_SETTINGS_NAMES*| Get the list of the name of all local settings. |**CONFIG_ID**|**SETTINGS_NAME** (list of settings names)| 0 |
|*CONFIG_GET_SETTINGS*| Get the list of all local settings. |**CONFIG_ID**|**SETTINGS** (map of settings : "SETTING_NAME" -> "SETTING_VALUE")| 0 |
|*CONFIG_GET_ALIASES*| Get the list of all local aliases. |**CONFIG_ID**|**ALIASES** (map of aliases : "ALIAS_NAME" -> "SETTING_NAME")| 0 |
|*CONFIG_INCLUDE*| Include a config |**CONFIG_ID**<br>**SRC** (a config_id of config to include)<br>**INCLUDE_POSITION** (position in the list of inclusion, where 0 is the first to be included. Negative values can be used, and then position will be *size* decreased by value. If not specified, is equal to *-1*)|*none*| 0 |
|*CONFIG_UNINCLUDE*| Uninclude a config |**CONFIG_ID**<br>**SRC** (a config_id corresponding to the wanted config) *or* <br>**INDEX** (position in the list of inclusion, working like in *CONFIG_INCLUDE*)|*none*| 0 |
|*SETTING_UPDATE*| Update or create setting |**CONFIG_ID**<br>**SETTINGS_TO_UPDATE**|*none*| 0 |
|*SETTING_REMOVE*| Remove setting |**CONFIG_ID**<br>**SETTING_NAME**|*none*| 0 |
|*SETTING_GET*| Get setting |**CONFIG_ID**<br>**SETTING_NAME**|**SETTING_VALUE**| 0 |
|*ALIAS_SET*| Update or create alias |**CONFIG_ID**<br>**SETTING_NAME**<br>**ALIAS_NAME**|*none*| 0 |
|*ALIAS_UNSET*| Unset alias |**CONFIG_ID**<br>**ALIAS_NAME**|*none*| 0 |
|*SUBSCRIBE_SETTING*| Subscribe to given setting |**CONFIG_ID**<br>**SETTING_NAME** *or* **ALIAS_NAME**|*none*| 0 |
|*UNSUBSCRIBE_SETTING*| Unsubscribe from given setting |**CONFIG_ID**<br>**SETTING_NAME** *or* **ALIAS_NAME**|*none*| 0 |

### REQUEST_STATE

| Value | Meaning |
| -------- | -------- |
| SUCCESS | Everything ok |
| BAD_ORDER | Argument(s) missing or bad |
| UNKNOWN_REQUEST | Bad request name |
| INTERNAL_ERROR | The service got an error |
| UNAUTHORIZED | Sent if the client tries to modify a config using a read-only key |
| UNKNOWN_ID | Given config id doesn't exist|
| UNKNOWN_KEY | Given config key doesn't exist|
| UNKNOWN_SETTING | Given setting name doesn't exist|
| UNKNOWN_ALIAS | Given alias name doesn't exist|

## Events

The service can also send a message to the lib to see if a subscribed setting is modified.
All events must contain "SETTING_NAME" containing the name of the concerned setting and "SUBSCRIPTION_EVENT_TYPE" containing one of the following values :

| Value | Description |
| ---- | ---- |
| *UPDATE* | The value has changed |
| *DELETE* | The setting has been deleted |
