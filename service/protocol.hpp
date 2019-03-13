//
// Created by milerius on 20/10/18.
//

#pragma once

#include <string>
#include <json.hpp>
#include "service_strong_types.hpp"

namespace raven
{
  enum class request_state : short
  {
    success,
    bad_order,
    unknown_request,
    internal_error,
    unauthorized,
    unknown_id,
    unknown_key,
    unknown_setting,
    unknown_alias,
    db_error = -1
  };

  inline const std::unordered_map<request_state, std::string> convert_request_state
      {
          {request_state::success,         "SUCCESS"},
          {request_state::bad_order,       "BAD_ORDER"},
          {request_state::unknown_request, "UNKNOWN_REQUEST"},
          {request_state::internal_error,  "INTERNAL_ERROR"},
          {request_state::unauthorized,    "UNAUTHORIZED"},
          {request_state::unknown_id,      "UNKNOWN_ID"},
          {request_state::unknown_key,     "UNKNOWN_KEY"},
          {request_state::unknown_setting, "UNKNOWN_SETTING"},
          {request_state::unknown_alias,   "UNKNOWN_ALIAS"},
          {request_state::db_error,        "DB_ERROR"},
      };

  namespace json = nlohmann;

  //! Keywords
  inline constexpr const char request_keyword[] = "REQUEST_NAME";
  inline constexpr const char request_state_keyword[] = "REQUEST_STATE";

  //! Protocol Constants
  inline constexpr const char config_name_keyword[] = "CONFIG_NAME";
  inline constexpr const char config_key_keyword[] = "CONFIG_KEY";
  inline constexpr const char config_read_only_key_keyword[] = "READONLY_CONFIG_KEY";
  inline constexpr const char config_id[] = "CONFIG_ID";
  inline constexpr const char config_include_src[] = "SRC";
  inline constexpr const char config_include_dst[] = "DST";
  inline constexpr const char setting_name[] = "SETTING_NAME";
  inline constexpr const char settings_to_update_keyword[] = "SETTINGS_TO_UPDATE";
  //inline constexpr const char setting_value[] = "SETTING_VALUE";
  inline constexpr const char alias_name[] = "ALIAS_NAME";

  //! CONFIG_CREATE
  struct config_create
  {
    std::string config_name;
  };

  inline void from_json(const raven::json::json &json_data, config_create &cfg)
  {
      cfg.config_name = json_data.at(config_name_keyword).get<std::string>();
  }

  //! CONFIG_CREATE ANSWER
  struct config_create_answer
  {
    config_key_st config_key;
    config_key_st readonly_config_key;
    std::string request_state;
  };

  void to_json(raven::json::json &json_data, const config_create_answer &cfg)
  {
      json_data = {{"CONFIG_KEY",          cfg.config_key.value()},
                   {"READONLY_CONFIG_KEY", cfg.readonly_config_key.value()},
                   {"REQUEST_STATE",       cfg.request_state}};
  }

  //! CONFIG_LOAD
  struct config_load
  {
    std::optional<config_key_st> config_key{std::nullopt};
    std::optional<config_key_st> config_read_only_key{std::nullopt};
  };

  inline void from_json(const raven::json::json &json_data, config_load &cfg)
  {
      //! We are checking if the config key keyword is present, otherwise it's probably a read only value
      if (json_data.count(config_key_keyword) > 0) {
          cfg.config_key = config_key_st{json_data.at(config_key_keyword).get<std::string>()};
      } else if (json_data.count(config_read_only_key_keyword) > 0) {
          cfg.config_read_only_key = config_key_st{json_data.at(config_read_only_key_keyword).get<std::string>()};
      }
  }

  //! CONFIG_LOAD ANSWER
  struct config_load_answer
  {
    std::string config_name;
    config_id_st config_id;
    std::string request_state;
  };

  void to_json(raven::json::json &json_data, const config_load_answer &cfg)
  {
      json_data = {{"CONFIG_NAME",   cfg.config_name},
                   {"CONFIG_ID",     cfg.config_id.value()},
                   {"REQUEST_STATE", cfg.request_state}};
  }

  //! CONFIG_UNLOAD
  struct config_unload
  {
    std::uint32_t id;
  };

  inline void from_json(const raven::json::json &json_data, config_unload &cfg)
  {
      cfg.id = json_data.at(config_id).get<std::uint32_t>();
  }

  //! CONFIG_INCLUDE
  struct config_include
  {
    std::uint32_t id;
    std::string src;
    std::string dst;
  };

  inline void from_json(const raven::json::json &json_data, config_include &cfg)
  {
      cfg.id = json_data.at(config_id).get<std::uint32_t>();
      cfg.src = json_data.at(config_include_src).get<std::string>();
      cfg.dst = json_data.at(config_include_dst).get<std::string>();
  }

  //! SETTING_UPDATE
  struct setting_update
  {
    config_id_st id;
    json::json settings_to_update{json::json::object()};
  };

  inline void from_json(const raven::json::json &json_data, setting_update &cfg)
  {
      cfg.id = config_id_st{json_data.at(config_id).get<int>()};
      cfg.settings_to_update = json_data.at(settings_to_update_keyword);
  }

  //! SETTING_REMOVE
  struct setting_remove
  {
    std::uint32_t id;
    std::string setting_name;
  };

  inline void from_json(const raven::json::json &json_data, setting_remove &cfg)
  {
      cfg.id = json_data.at(config_id).get<std::uint32_t>();
      cfg.setting_name = json_data.at(setting_name).get<std::string>();
  }

  //! SETTING_GET
  struct setting_get
  {
    std::uint32_t id;
    std::string setting_name;
  };

  inline void from_json(const raven::json::json &json_data, setting_get &cfg)
  {
      cfg.id = json_data.at(config_id).get<std::uint32_t>();
      cfg.setting_name = json_data.at(setting_name).get<std::string>();
  }

  //! SETTING_GET ANSWER
  struct setting_get_answer
  {
    std::string setting_value;
    std::string request_state;
  };

  void to_json(raven::json::json &json_data, const setting_get_answer &cfg)
  {
      json_data = {{"SETTING_VALUE", cfg.setting_value},
                   {"REQUEST_STATE", cfg.request_state}};
  }

  //! ALIAS_SET
  struct alias_set
  {
    std::uint32_t id;
    std::string setting_name;
    std::string alias_name;
  };

  inline void from_json(const raven::json::json &json_data, alias_set &cfg)
  {
      cfg.id = json_data.at(config_id).get<std::uint32_t>();
      cfg.setting_name = json_data.at(setting_name).get<std::string>();
      cfg.alias_name = json_data.at(alias_name).get<std::string>();
  }

  //! ALIAS_UNSET
  struct alias_unset
  {
    std::uint32_t id;
    std::string alias_name;
  };

  inline void from_json(const raven::json::json &json_data, alias_unset &cfg)
  {
      cfg.id = json_data.at(config_id).get<std::uint32_t>();
      cfg.alias_name = json_data.at(alias_name).get<std::string>();
  }

  template <typename TSetting>
  void fill_subscription_struct(const raven::json::json &json_data, TSetting &&cfg)
  {
      cfg.id = json_data.at(config_id).get<std::uint32_t>();
      if (json_data.count(alias_name) > 0) {
          cfg.alias_name = json_data.at(alias_name).get<std::string>();
      } else if (json_data.count(setting_name) > 0) {
          cfg.setting_name = json_data.at(setting_name).get<std::string>();
      }
  }

  //! SUBSCRIBE_SETTING
  struct setting_subscribe
  {
    std::uint32_t id;
    std::optional<std::string> setting_name{std::nullopt};
    std::optional<std::string> alias_name{std::nullopt};
  };

  inline void from_json(const raven::json::json &json_data, setting_subscribe &cfg)
  {
      fill_subscription_struct<setting_subscribe>(json_data, std::forward<setting_subscribe>(cfg));
  }

  //! UNSUBSCRIBE_SETTING
  struct setting_unsubscribe
  {
    std::uint32_t id;
    std::optional<std::string> setting_name{std::nullopt};
    std::optional<std::string> alias_name{std::nullopt};
  };

  inline void from_json(const raven::json::json &json_data, setting_unsubscribe &cfg)
  {
      fill_subscription_struct<setting_unsubscribe>(json_data, std::forward<setting_unsubscribe>(cfg));
  }
}