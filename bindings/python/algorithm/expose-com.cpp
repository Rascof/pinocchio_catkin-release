//
// Copyright (c) 2015-2018 CNRS
//

#include "pinocchio/bindings/python/algorithm/algorithms.hpp"
#include "pinocchio/algorithm/center-of-mass.hpp"

#include <boost/python/overloads.hpp>

namespace pinocchio
{
  namespace python
  {

    BOOST_PYTHON_FUNCTION_OVERLOADS(jacobianCenterOfMassUpdate_overload,jacobianCenterOfMass,3,4)

    BOOST_PYTHON_FUNCTION_OVERLOADS(jacobianCenterOfMassNoUpdate_overload,jacobianCenterOfMass,2,3)

    static SE3::Vector3
    com_0_proxy(const Model& model,
                Data & data,
                const Eigen::VectorXd & q,
                bool computeSubtreeComs = true)
    {
      return centerOfMass(model,data,q,computeSubtreeComs);
    }

    static SE3::Vector3
    com_1_proxy(const Model& model,
                Data & data,
                const Eigen::VectorXd & q,
                const Eigen::VectorXd & v,
                bool computeSubtreeComs = true)
    {
      return centerOfMass(model,data,q,v,computeSubtreeComs);
    }

    static SE3::Vector3
    com_2_proxy(const Model & model,
                Data & data,
                const Eigen::VectorXd & q,
                const Eigen::VectorXd & v,
                const Eigen::VectorXd & a,
                bool computeSubtreeComs = true)
    {
      return centerOfMass(model,data,q,v,a,computeSubtreeComs);
    }

    static void
    com_level_proxy(const Model & model,
                    Data & data,
                    int LEVEL,
                    bool computeSubtreeComs = true)
    {
      centerOfMass(model,data,LEVEL,computeSubtreeComs);
    }

    static void
    com_default_proxy(const Model & model,
                Data & data,
                bool computeSubtreeComs = true)
    {
      centerOfMass(model,data,computeSubtreeComs);
    }

    static Data::Matrix3x
    jacobian_subtree_com_kinematics_proxy(const Model & model,
                                          Data & data,
                                          const Eigen::VectorXd & q,
                                          Model::JointIndex jointId)
    {
      Data::Matrix3x J(3,model.nv); J.setZero();
      jacobianSubtreeCenterOfMass(model, data, q, jointId, J);

      return J;
    }

    static Data::Matrix3x
    jacobian_subtree_com_proxy(const Model & model,
                               Data & data,
                               Model::JointIndex jointId)
    {
      Data::Matrix3x J(3,model.nv); J.setZero();
      jacobianSubtreeCenterOfMass(model, data, jointId, J);

      return J;
    }
    
    static Data::Matrix3x
    get_jacobian_subtree_com_proxy(const Model & model,
                                   Data & data,
                                   Model::JointIndex jointId)
    {
      Data::Matrix3x J(3,model.nv); J.setZero();
      getJacobianSubtreeCenterOfMass(model, data, jointId, J);
      
      return J;
    }

    BOOST_PYTHON_FUNCTION_OVERLOADS(com_0_overload, com_0_proxy, 3, 4)

    BOOST_PYTHON_FUNCTION_OVERLOADS(com_1_overload, com_1_proxy, 4, 5)

    BOOST_PYTHON_FUNCTION_OVERLOADS(com_2_overload, com_2_proxy, 5, 6)

    BOOST_PYTHON_FUNCTION_OVERLOADS(com_level_overload, com_level_proxy, 3, 4)

    BOOST_PYTHON_FUNCTION_OVERLOADS(com_default_overload, com_default_proxy, 2, 3)

    void exposeCOM()
    {
      using namespace Eigen;

      bp::def("computeTotalMass",
              (double (*)(const Model &))&computeTotalMass<double,0,JointCollectionDefaultTpl>,
              bp::args("Model"),
              "Compute the total mass of the model and return it.");

      bp::def("computeTotalMass",
              (double (*)(const Model &, Data &))&computeTotalMass<double,0,JointCollectionDefaultTpl>,
              bp::args("Model", "Data"),
              "Compute the total mass of the model, put it in data.mass[0] and return it.");

      bp::def("computeSubtreeMasses",
              (void (*)(const Model &, Data &))&computeSubtreeMasses<double,0,JointCollectionDefaultTpl>,
              bp::args("Model", "Data"),
              "Compute the mass of each kinematic subtree and store it in data.mass.");

      bp::def("centerOfMass",
              com_0_proxy,
              com_0_overload(
                  bp::args("Model","Data",
                           "Joint configuration q (size Model::nq)",
                           "computeSubtreeComs If true, the algorithm computes also the center of mass of the subtrees"
                  ),
                  "Compute the center of mass, putting the result in Data and return it."
              )[bp::return_value_policy<bp::return_by_value>()]
      );

      bp::def("centerOfMass",
              com_1_proxy,
              com_1_overload(
                  bp::args("Model","Data",
                           "Joint configuration q (size Model::nq)",
                           "Joint velocity v (size Model::nv)",
                           "computeSubtreeComs If true, the algorithm computes also the center of mass of the subtrees"
                  ),
                  "Computes the center of mass position and velocity by storing the result in Data."
              )[bp::return_value_policy<bp::return_by_value>()]
      );

      bp::def("centerOfMass",
              com_2_proxy,
              com_2_overload(
                  bp::args("Model","Data",
                           "Joint configuration q (size Model::nq)",
                           "Joint velocity v (size Model::nv)",
                           "Joint acceleration a (size Model::nv)",
                           "computeSubtreeComs If true, the algorithm computes also the center of mass of the subtrees"
                  ),
                  "Computes the center of mass position, velocity and acceleration by storing the result in Data."
              )[bp::return_value_policy<bp::return_by_value>()]
      );

      bp::def("centerOfMass",
              com_level_proxy,
              com_level_overload(
                  bp::args("Model","Data",
                           "level if = 0, computes CoM position, if = 1, also computes CoM velocity and if = 2, also computes CoM acceleration",
                           "computeSubtreeComs If true, the algorithm computes also the center of mass of the subtrees"
                  ),
                  "Computes the center of mass position, velocity and acceleration of a given model according to the current kinematic values contained in data and the requested level."
              )[bp::return_value_policy<bp::return_by_value>()]
      );

      bp::def("centerOfMass",
              com_default_proxy,
              com_default_overload(
                  bp::args("Model","Data",
                           "computeSubtreeComs If true, the algorithm computes also the center of mass of the subtrees"
                  ),
                  "Computes the center of mass position, velocity and acceleration of a given model according to the current kinematic values contained in data."
              )[bp::return_value_policy<bp::return_by_value>()]
      );

      bp::def("jacobianCenterOfMass",
              (const Data::Matrix3x & (*)(const Model &, Data &, const Eigen::MatrixBase<Eigen::VectorXd> &, bool))&jacobianCenterOfMass<double,0,JointCollectionDefaultTpl,VectorXd>,
              jacobianCenterOfMassUpdate_overload(bp::args("Model","Data",
                       "Joint configuration q (size Model::nq)",
                       "computeSubtreeComs If true, the algorithm computes also the center of mass of the subtrees"),
              "Computes the jacobian of the center of mass, puts the result in Data and return it.")[
              bp::return_value_policy<bp::return_by_value>()]);

      bp::def("jacobianCenterOfMass",
              (const Data::Matrix3x & (*)(const Model &, Data &, bool))&jacobianCenterOfMass<double,0,JointCollectionDefaultTpl>,
              jacobianCenterOfMassNoUpdate_overload(bp::args("Model","Data",
                       "computeSubtreeComs If true, the algorithm computes also the center of mass of the subtrees"),
              "Computes the jacobian of the center of mass, puts the result in Data and return it.")[
              bp::return_value_policy<bp::return_by_value>()]);

      bp::def("jacobianSubtreeCoMJacobian",jacobian_subtree_com_kinematics_proxy,
              bp::args("Model, the model of the kinematic tree",
                       "Data, the data associated to the model where the results are stored",
                       "Joint configuration q (size Model::nq)",
                       "Subtree root ID, the index of the subtree root joint."),
              "Computes the Jacobian of the CoM of the given subtree expressed in the world frame, according to the given joint configuration.");

      bp::def("jacobianSubtreeCoMJacobian",jacobian_subtree_com_proxy,
              bp::args("Model, the model of the kinematic tree",
                       "Data, the data associated to the model where the results are stored",
                       "Subtree root ID, the index of the subtree root joint."),
              "Computes the Jacobian of the CoM of the given subtree expressed in the world frame, according to the given entries in data.");
      
      bp::def("getJacobianSubtreeCenterOfMass",get_jacobian_subtree_com_proxy,
              bp::args("Model, the model of the kinematic tree",
                       "Data, the data associated to the model where the results are stored",
                       "Subtree root ID, the index of the subtree root joint."),
              "Get the Jacobian of the CoM of the given subtree expressed in the world frame, according to the given entries in data. It assumes that jacobianCenterOfMass has been called first.");

    }

  } // namespace python
} // namespace pinocchio
