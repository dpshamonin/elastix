/*======================================================================

  This file is part of the elastix software.

  Copyright (c) University Medical Center Utrecht. All rights reserved.
  See src/CopyrightElastix.txt or http://elastix.isi.uu.nl/legal.php for
  details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE. See the above copyright notices for more information.

======================================================================*/
#ifndef __elxTransformRigidityPenaltyTerm_H__
#define __elxTransformRigidityPenaltyTerm_H__

#include "elxIncludes.h"
#include "itkTransformRigidityPenaltyTerm.h"

#include "elxTimer.h"

namespace elastix
{
using namespace itk;

  /**
   * \class TransformRigidityPenalty
   * \brief A penalty term based on non-rigidity.
   *
   * The parameters used in this class are:
   * \parameter Metric: Select this metric as follows:\n
   *    <tt>(Metric "TransformRigidityPenalty")</tt>
   *
   * \ingroup Metrics
   *
   */

template <class TElastix >
class TransformRigidityPenalty
  : public TransformRigidityPenaltyTerm<
  ITK_TYPENAME MetricBase<TElastix>::FixedImageType,
  double >,
  public MetricBase<TElastix>
{
public:

  /** Standard ITK-stuff. */
  typedef TransformRigidityPenalty                      Self;
  typedef TransformRigidityPenaltyTerm<
    typename MetricBase<TElastix>::FixedImageType,
    double >                                            Superclass1;
  typedef MetricBase<TElastix>                          Superclass2;
  typedef SmartPointer<Self>                            Pointer;
  typedef SmartPointer<const Self>                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information (and related methods). */
  itkTypeMacro( TransformRigidityPenalty, TransformRigidityPenaltyTerm );

  /** Name of this class.
   * Use this name in the parameter file to select this specific metric. \n
   * example: <tt>(Metric "TransformRigidityPenalty")</tt>\n
   */
  elxClassNameMacro( "TransformRigidityPenalty" );

  /** Typedefs from the superclass. */
  typedef typename Superclass1::CoordinateRepresentationType CoordinateRepresentationType;
  typedef typename Superclass1::MovingImageType             MovingImageType;
  typedef typename Superclass1::MovingImagePixelType        MovingImagePixelType;
  typedef typename Superclass1::MovingImageConstPointer     MovingImageConstPointer;
  typedef typename Superclass1::FixedImageType              FixedImageType;
  typedef typename Superclass1::FixedImageConstPointer      FixedImageConstPointer;
  typedef typename Superclass1::FixedImageRegionType        FixedImageRegionType;
  typedef typename Superclass1::TransformType               TransformType;
  typedef typename Superclass1::TransformPointer            TransformPointer;
  typedef typename Superclass1::InputPointType              InputPointType;
  typedef typename Superclass1::OutputPointType             OutputPointType;
  typedef typename Superclass1::TransformParametersType     TransformParametersType;
  typedef typename Superclass1::TransformJacobianType       TransformJacobianType;
  typedef typename Superclass1::InterpolatorType            InterpolatorType;
  typedef typename Superclass1::InterpolatorPointer         InterpolatorPointer;
  typedef typename Superclass1::RealType                    RealType;
  typedef typename Superclass1::GradientPixelType           GradientPixelType;
  typedef typename Superclass1::GradientImageType           GradientImageType;
  typedef typename Superclass1::GradientImagePointer        GradientImagePointer;
  typedef typename Superclass1::GradientImageFilterType     GradientImageFilterType;
  typedef typename Superclass1::GradientImageFilterPointer  GradientImageFilterPointer;
  typedef typename Superclass1::FixedImageMaskType          FixedImageMaskType;
  typedef typename Superclass1::FixedImageMaskPointer       FixedImageMaskPointer;
  typedef typename Superclass1::MovingImageMaskType         MovingImageMaskType;
  typedef typename Superclass1::MovingImageMaskPointer      MovingImageMaskPointer;
  typedef typename Superclass1::MeasureType                 MeasureType;
  typedef typename Superclass1::DerivativeType              DerivativeType;
  typedef typename Superclass1::ParametersType              ParametersType;
  typedef typename Superclass1::FixedImagePixelType         FixedImagePixelType;
  typedef typename Superclass1::MovingImageRegionType       MovingImageRegionType;
  typedef typename Superclass1::ImageSamplerType            ImageSamplerType;
  typedef typename Superclass1::ImageSamplerPointer         ImageSamplerPointer;
  typedef typename Superclass1::ImageSampleContainerType    ImageSampleContainerType;
  typedef typename Superclass1::ImageSampleContainerPointer ImageSampleContainerPointer;
  typedef typename Superclass1::FixedImageLimiterType       FixedImageLimiterType;
  typedef typename Superclass1::MovingImageLimiterType      MovingImageLimiterType;
  typedef typename Superclass1::FixedImageLimiterOutputType FixedImageLimiterOutputType;
  typedef typename Superclass1::MovingImageLimiterOutputType MovingImageLimiterOutputType;

  typedef typename Superclass1::CoefficientImageType        CoefficientImageType;

  /** The fixed image dimension. */
  itkStaticConstMacro( FixedImageDimension, unsigned int,
    FixedImageType::ImageDimension );

  /** The moving image dimension. */
  itkStaticConstMacro( MovingImageDimension, unsigned int,
    MovingImageType::ImageDimension );

  /** Typedef's inherited from elastix. */
  typedef typename Superclass2::ElastixType               ElastixType;
  typedef typename Superclass2::ElastixPointer            ElastixPointer;
  typedef typename Superclass2::ConfigurationType         ConfigurationType;
  typedef typename Superclass2::ConfigurationPointer      ConfigurationPointer;
  typedef typename Superclass2::RegistrationType          RegistrationType;
  typedef typename Superclass2::RegistrationPointer       RegistrationPointer;
  typedef typename Superclass2::ITKBaseType               ITKBaseType;

  /** Typedef for timer. */
  typedef tmr::Timer          TimerType;
  /** Typedef for timer. */
  typedef TimerType::Pointer  TimerPointer;

  /** Sets up a timer to measure the initialization time and
   * calls the Superclass' implementation.
   */
  virtual void Initialize( void ) throw (ExceptionObject);

  /**
   * Do some things before each resolution:
   * \li Read all parameters.
   */
  virtual void BeforeEachResolution( void );

  /**
   * Do some things before registration:
   * \li Read the fixed rigidity image.
   * \li Read the moving rigidity image.
   * \li Setup some extra target cells.
   */
  virtual void BeforeRegistration( void );

  /**
   * Do some things after each iteration:
   * \li Print the OC, PC, LC parts of the rigidity term.
   */
  virtual void AfterEachIteration( void );

  /** This metric is advanced (so it has a sampling possibility), but it
   * purposely does not use samplers. The MetricBase class, however, issues
   * a warning if this is the case, so we overwrite that function.
   */
  virtual void SelectNewSamples( void ){};

protected:

  /** The constructor. */
  TransformRigidityPenalty(){};

  /** The destructor. */
  virtual ~TransformRigidityPenalty() {}

private:

  /** The private constructor. */
  TransformRigidityPenalty( const Self& ); // purposely not implemented
  /** The private copy constructor. */
  void operator=( const Self& );              // purposely not implemented

}; // end class TransformRigidityPenalty


} // end namespace elastix


#ifndef ITK_MANUAL_INSTANTIATION
#include "elxTransformRigidityPenaltyTerm.hxx"
#endif

#endif // end #ifndef __elxTransformRigidityPenaltyTerm_H__
