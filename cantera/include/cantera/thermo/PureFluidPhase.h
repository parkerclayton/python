/**
 *  @file PureFluidPhase.h
 *
 *   Header for a ThermoPhase class for a pure fluid phase consisting of
 *   gas, liquid, mixed-gas-liquid and supercrit fluid (see \ref thermoprops
 *   and class \link Cantera::PureFluidPhase PureFluidPhase\endlink).
 *
 * It inherits from ThermoPhase, but is built on top of the tpx package.
 */

//  Copyright 2003 California Institute of Technology

#ifndef CT_EOS_TPX_H
#define CT_EOS_TPX_H

#include "ThermoPhase.h"
#include "mix_defs.h"
#include "cantera/tpx/Sub.h"

namespace Cantera
{
//! This phase object consists of a single component that can be a gas, a
//! liquid, a mixed gas-liquid fluid, or a fluid beyond its critical point
/*!
 * The object inherits from ThermoPhase. However, it's built on top of the tpx
 * package.
 *
 * @ingroup thermoprops
 */
class PureFluidPhase : public ThermoPhase
{
public:
    //! Empty Base Constructor
    PureFluidPhase();

    PureFluidPhase(const PureFluidPhase& right);
    PureFluidPhase& operator=(const PureFluidPhase& right);
    virtual ThermoPhase* duplMyselfAsThermoPhase() const;

    //! Equation of state type
    virtual int eosType() const {
        return cPureFluid;
    }

    virtual doublereal enthalpy_mole() const;
    virtual doublereal intEnergy_mole() const;
    virtual doublereal entropy_mole() const;
    virtual doublereal gibbs_mole() const;
    virtual doublereal cp_mole() const;
    virtual doublereal cv_mole() const;

    //! Return the thermodynamic pressure (Pa).
    /*!
     * This method calculates the current pressure consistent with the
     * independent variables, T, rho.
     */
    virtual doublereal pressure() const;

    //! sets the thermodynamic pressure (Pa).
    /*!
     * This method calculates the density that is consistent with the
     * desired pressure, given the temperature.
     *
     * @param p  Pressure (Pa)
     */
    virtual void setPressure(doublereal p);

    virtual void getChemPotentials(doublereal* mu) const {
        mu[0] = gibbs_mole();
    }

    virtual void getPartialMolarEnthalpies(doublereal* hbar) const;
    virtual void getPartialMolarEntropies(doublereal* sbar) const;
    virtual void getPartialMolarIntEnergies(doublereal* ubar) const;
    virtual void getPartialMolarCp(doublereal* cpbar) const;
    virtual void getPartialMolarVolumes(doublereal* vbar) const;

    virtual void getActivityConcentrations(doublereal* c) const;
    virtual doublereal standardConcentration(size_t k=0) const;

    virtual void getActivities(doublereal* a) const;

    virtual doublereal isothermalCompressibility() const;
    virtual doublereal thermalExpansionCoeff() const;

    //! Returns a reference to the substance object
    tpx::Substance& TPX_Substance();

    //@}
    /// @name Properties of the Standard State of the Species in the Solution
    /*!
     *  The standard state of the pure fluid is defined as the real properties
     *  of the pure fluid at the most stable state of the fluid at the current
     *  temperature and pressure of the solution. With this definition, the
     *  activity of the fluid is always then defined to be equal to one.
     */
    //@{

    virtual void getStandardChemPotentials(doublereal* mu) const;
    virtual void getEnthalpy_RT(doublereal* hrt) const;
    virtual void getEntropy_R(doublereal* sr) const;
    virtual void getGibbs_RT(doublereal* grt) const;

    //@}

    /// @name Thermodynamic Values for the Species Reference States
    /*!
     * The species reference state for pure fluids is defined as an ideal gas at
     * the reference pressure and current temperature of the fluid.
     */
    //@{

    virtual void getEnthalpy_RT_ref(doublereal* hrt) const;
    virtual void getGibbs_RT_ref(doublereal* grt) const;
    virtual void getGibbs_ref(doublereal* g) const;
    virtual void getEntropy_R_ref(doublereal* er) const;

    /**
     * @name Setting the State
     *
     * These methods set all or part of the thermodynamic state.
     * @{
     */

    virtual void setState_HP(double h, double p, double tol = 1.e-8);
    virtual void setState_UV(double u, double v, double tol = 1.e-8);
    virtual void setState_SV(double s, double v, double tol = 1.e-8);
    virtual void setState_SP(double s, double p, double tol = 1.e-8);
    virtual void setState_ST(double s, double t, double tol = 1.e-8);
    virtual void setState_TV(double t, double v, double tol = 1.e-8);
    virtual void setState_PV(double p, double v, double tol = 1.e-8);
    virtual void setState_UP(double u, double p, double tol = 1.e-8);
    virtual void setState_VH(double v, double h, double tol = 1.e-8);
    virtual void setState_TH(double t, double h, double tol = 1.e-8);
    virtual void setState_SH(double s, double h, double tol = 1.e-8);
    //@}

    //! @name Critical State Properties
    //@{

    virtual doublereal critTemperature() const;
    virtual doublereal critPressure() const;
    virtual doublereal critDensity() const;

    //@}

    //! @name Saturation properties.
    //@{

    virtual doublereal satTemperature(doublereal p) const;
    virtual doublereal satPressure(doublereal t);
    virtual doublereal vaporFraction() const;

    virtual void setState_Tsat(doublereal t, doublereal x);
    virtual void setState_Psat(doublereal p, doublereal x);
    //@}

    virtual void initThermo();
    virtual void setParametersFromXML(const XML_Node& eosdata);

    virtual std::string report(bool show_thermo=true,
                               doublereal threshold=1e-14) const;

protected:
    //! Main call to the tpx level to set the state of the system
    /*!
     * @param n  Integer indicating which 2 thermo components are held constant
     * @param x  Value of the first component
     * @param y  Value of the second component
     */
    void Set(tpx::PropertyPair::type n, double x, double y) const;

    //! Sets the state using a TPX::TV call
    void setTPXState() const;

private:
    //! Pointer to the underlying tpx object Substance that does the work
    mutable std::unique_ptr<tpx::Substance> m_sub;

    //! Int indicating the type of the fluid
    /*!
     * The tpx package uses an int to indicate what fluid is being sought.
     */
    int m_subflag;

    //! Molecular weight of the substance (kg kmol-1)
    doublereal m_mw;

    //! flag to turn on some printing.
    bool m_verbose;
};

}

#endif
