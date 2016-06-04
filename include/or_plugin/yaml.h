///
/// \file or_plugin.h
/// \brief Helpers for converting OpenRAVE datatypes to and from YAML.
///
#ifndef OR_PLUGIN_YAML_H
#define OR_PLUGIN_YAML_H

#include <openrave/openrave.h>
#include <yaml-cpp/yaml.h>
#include <string>

namespace YAML {

///
/// YAML Converter for OpenRAVE Kinbodies
///
/// Kinbodies are stored in the format:
/// [ env_id, kinbody_name ]
/// 
template<>
struct convert<OpenRAVE::KinBodyPtr>
{
  static Node encode(const OpenRAVE::KinBodyPtr kinbody)
  {
    Node node;
    node.push_back(kinbody->GetEnvironmentId());
    node.push_back(kinbody->GetName());
    return node;
  }

  static bool decode(const Node& node, OpenRAVE::KinBodyPtr &kinbody)
  {
    if(!node.IsSequence() || node.size() != 2)
    {
      RAVELOG_WARN("Kinbody must have format [env, kinbody].");
      return false;
    }

    int env_id = node[0].as<int>();
    OpenRAVE::EnvironmentBasePtr env = OpenRAVE::RaveGetEnvironment(env_id);
    if (!env)
    {
      RAVELOG_WARN("Invalid environment ID: %d.\n", env_id);
      return false;
    }

    const std::string kinbody_name = node[1].as<std::string>();
    kinbody = env->GetKinBody(kinbody_name);
    if (!kinbody)
    {
      RAVELOG_WARN("No kinbody '%s' found in environment '%d'.\n",
                   kinbody_name.c_str(), env_id);
      return false;
    }

    return true;
  }
};

///
/// YAML Converter for OpenRAVE Links
///
/// Links are stored in the format:
/// [ env_id, kinbody_name, link_name ]
/// 
template<>
struct convert<OpenRAVE::KinBody::LinkPtr>
{
  static Node encode(const OpenRAVE::KinBody::LinkPtr link)
  {
    const OpenRAVE::KinBodyPtr kinbody = link->GetParent();

    Node node;
    node.push_back(kinbody->GetEnvironmentId());
    node.push_back(kinbody->GetName());
    node.push_back(link->GetName());
    return node;
  }

  static bool decode(const Node& node, OpenRAVE::KinBody::LinkPtr &link)
  {
    if(!node.IsSequence() || node.size() != 3)
    {
      RAVELOG_ERROR("Link must have format [env, kinbody, link].\n");
      return false;
    }

    int env_id = node[0].as<int>();
    OpenRAVE::EnvironmentBasePtr env = OpenRAVE::RaveGetEnvironment(env_id);
    if (!env)
    {
      RAVELOG_ERROR("Invalid environment ID: %d.\n", env_id);
      return false;
    }

    const std::string kinbody_name = node[1].as<std::string>();
    OpenRAVE::KinBodyPtr kinbody = env->GetKinBody(kinbody_name);
    if (!kinbody)
    {
      RAVELOG_ERROR("No kinbody '%s' in environment '%d'.\n",
                   kinbody_name.c_str(), env_id);
      return false;
    }

    const std::string link_name = node[2].as<std::string>();
    link = kinbody->GetLink(link_name);
    if (!link)
    {
      RAVELOG_ERROR("No link '%s' in kinbody '%s' in environment '%d'.\n",
                   link_name.c_str(), kinbody_name.c_str(), env_id);
      return false;
    }

    return true;
  }
};

///
/// YAML Converter for OpenRAVE Transforms
///
/// Transforms are stored as a list of lists in row-major form:
/// [[m00, m01, m02, m03],
///  [m10, m11, m12, m13],
///  [m20, m21, m22, m23],
///  [m30, m31, m32, m33]]
///
template<>
struct convert<OpenRAVE::Transform>
{
  static Node encode(const OpenRAVE::Transform kinbody)
  {
    Node node;
    // TODO: implement this.
    return node;
  }

  static bool decode(const Node& node, OpenRAVE::Transform &kinbody)
  {
    if(!node.IsSequence() || node.size() != 4)
    {
      RAVELOG_ERROR("Transform must have format [[4x4]].\n");
      return false;
    }

    // TODO: implement this.

    return false;
  }
};

} // namespace YAML

#endif // OR_PLUGIN_YAML_H
