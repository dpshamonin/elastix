/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkCorrespondingPointsEuclideanDistancePointMetric.h,v $
  Language:  C++
  Date:      $Date: 2009-04-23 03:53:35 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkCorrespondingPointsEuclideanDistancePointMetric_h
#define __itkCorrespondingPointsEuclideanDistancePointMetric_h

#include "itkSingleValuedPointSetToPointSetMetric.h"
#include "itkPoint.h"
#include "itkPointSet.h"
#include "itkImage.h"

namespace itk
{

/** \class CorrespondingPointsEuclideanDistancePointMetric
 * \brief Computes the Euclidean distance between a moving point-set
 *  and a fixed point-set.
 *  Correspondence is needed.
 *
 *
 * \ingroup RegistrationMetrics
 */

template < class TFixedPointSet, class TMovingPointSet >
class ITK_EXPORT CorrespondingPointsEuclideanDistancePointMetric : 
    public SingleValuedPointSetToPointSetMetric<TFixedPointSet, TMovingPointSet>
{
public:

  /** Standard class typedefs. */
  typedef CorrespondingPointsEuclideanDistancePointMetric    Self;
  typedef SingleValuedPointSetToPointSetMetric<
    TFixedPointSet, TMovingPointSet >               Superclass;
  typedef SmartPointer<Self>                        Pointer;
  typedef SmartPointer<const Self>                  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );
 
  /** Run-time type information (and related methods). */
  itkTypeMacro( CorrespondingPointsEuclideanDistancePointMetric,
    SingleValuedPointSetToPointSetMetric );
 
  /** Types transferred from the base class */
  typedef typename Superclass::TransformType              TransformType;
  typedef typename Superclass::TransformPointer           TransformPointer;
  typedef typename Superclass::TransformParametersType    TransformParametersType;
  typedef typename Superclass::TransformJacobianType      TransformJacobianType;

  typedef typename Superclass::MeasureType                MeasureType;
  typedef typename Superclass::DerivativeType             DerivativeType;
  typedef typename Superclass::DerivativeValueType        DerivativeValueType;
  typedef typename Superclass::FixedPointSetType          FixedPointSetType;
  typedef typename Superclass::MovingPointSetType         MovingPointSetType;
  typedef typename Superclass::FixedPointSetConstPointer  FixedPointSetConstPointer;
  typedef typename Superclass::MovingPointSetConstPointer MovingPointSetConstPointer;

  typedef typename Superclass::PointIterator              PointIterator;
  typedef typename Superclass::PointDataIterator          PointDataIterator;

  typedef typename Superclass::InputPointType             InputPointType;
  typedef typename Superclass::OutputPointType            OutputPointType;
  typedef typename OutputPointType::CoordRepType          CoordRepType;
  typedef vnl_vector<CoordRepType>                        VnlVectorType;
  
  typedef typename Superclass::NonZeroJacobianIndicesType NonZeroJacobianIndicesType;

  /**  Get the value for single valued optimizers. */
  MeasureType GetValue( const TransformParametersType & parameters ) const;

  /** Get the derivatives of the match measure. */
  void GetDerivative( const TransformParametersType & parameters,
    DerivativeType & Derivative ) const;

  /**  Get value and derivatives for multiple valued optimizers. */
  void GetValueAndDerivative( const TransformParametersType & parameters,
    MeasureType& Value, DerivativeType& Derivative ) const;

  /** Set/Get if the distance should be squared. Default is true for computation speed */
//   itkSetMacro(ComputeSquaredDistance,bool);
//   itkGetConstMacro(ComputeSquaredDistance,bool);
//   itkBooleanMacro(ComputeSquaredDistance);

protected:
  CorrespondingPointsEuclideanDistancePointMetric();
  virtual ~CorrespondingPointsEuclideanDistancePointMetric() {};

  /** PrintSelf. */
  void PrintSelf(std::ostream& os, Indent indent) const;

private:
  CorrespondingPointsEuclideanDistancePointMetric(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  //bool               m_ComputeSquaredDistance;

}; // end class CorrespondingPointsEuclideanDistancePointMetric

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkCorrespondingPointsEuclideanDistancePointMetric.txx"
#endif

#endif
