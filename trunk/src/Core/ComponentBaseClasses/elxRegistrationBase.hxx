/*======================================================================

  This file is part of the elastix software.

  Copyright (c) University Medical Center Utrecht. All rights reserved.
  See src/CopyrightElastix.txt or http://elastix.isi.uu.nl/legal.php for
  details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE. See the above copyright notices for more information.

======================================================================*/

#ifndef __elxRegistrationBase_hxx
#define __elxRegistrationBase_hxx

#include "elxRegistrationBase.h"


namespace elastix
{
using namespace itk;

/**
 * ********************* ReadMaskParameters ************************
 */

template <class TElastix>
bool
RegistrationBase<TElastix>
::ReadMaskParameters( UseMaskErosionArrayType & useMaskErosionArray,
  const unsigned int nrOfMasks, const std::string & whichMask,
  const unsigned int level ) const
{    
  /** Read whether mask erosion is wanted, if any masks were supplied. */

  /** Bool that remembers if mask erosion is wanted in any of the masks 
   * remains false when no masks are used. This bool will be output.
   */
  bool useMaskErosion = false; 

  /** Array of bools, that remembers for each mask if erosion is wanted. */
  useMaskErosionArray.resize( nrOfMasks, false );

  /** "ErodeFixedMask" or "ErodeMovingMask". */
  std::string whichErodeMaskOption( "Erode" );
  whichErodeMaskOption += whichMask;
  whichErodeMaskOption += "Mask";

  /** Read the parameters. */
  if ( nrOfMasks > 0 )
  {
    /** Default values for all masks. Look for ErodeMask, or Erode<Fixed,Moving>Mask. */
    bool erosionOrNot = true;
    this->GetConfiguration()->ReadParameter( erosionOrNot,
      "ErodeMask", "", level, 0, false );
    this->GetConfiguration()->ReadParameter( erosionOrNot,
      whichErodeMaskOption.c_str(), "", level, 0 );
    if ( erosionOrNot )
    {
      /** fill with 'true's. */
      useMaskErosionArray.clear();
      useMaskErosionArray.resize( nrOfMasks, true );
    }
   
    /** Try to read an erode mask parameter given for a specified mask only:
     * (ErodeFixedMask0 "true" "false" ) for example.
     */
    for ( unsigned int i = 0; i < nrOfMasks; ++i )
    {
      std::ostringstream makestring;
      makestring << whichErodeMaskOption << i; // key for parameter file
      bool erosionOrNot_i = erosionOrNot; // default value
      this->GetConfiguration()->ReadParameter( erosionOrNot_i,
        makestring.str().c_str(), "", level, 0, false );
      if ( erosionOrNot_i )
      {
        useMaskErosionArray[ i ] = true;
      }
      else
      {
        useMaskErosionArray[ i ] = false;
      }
      /** Check if mask erosion is wanted in any of the masks. */
      useMaskErosion |= useMaskErosionArray[ i ];
    }
  } // end if nrOfMasks > 0

  return useMaskErosion;

} // end ReadMaskParameters()


/**
 * ******************* GenerateFixedMaskSpatialObject **********************
 */

template <class TElastix>
typename RegistrationBase<TElastix>::FixedMaskSpatialObjectPointer 
RegistrationBase<TElastix>
::GenerateFixedMaskSpatialObject(
  const FixedMaskImageType * maskImage, bool useMaskErosion,
  const FixedImagePyramidType * pyramid, unsigned int level ) const
{
  FixedMaskSpatialObjectPointer fixedMaskSpatialObject = 0;
  if ( !maskImage )
  {
    return fixedMaskSpatialObject;
  }
  fixedMaskSpatialObject = FixedMaskSpatialObjectType::New();

  /** Just convert to spatial object if no erosion is needed. */
  if ( !useMaskErosion || !pyramid )
  {
    fixedMaskSpatialObject->SetImage( maskImage );
    return fixedMaskSpatialObject;
  }

  /** Erode, and convert to spatial object. */
  FixedMaskErodeFilterPointer erosion = FixedMaskErodeFilterType::New();
  erosion->SetInput( maskImage );
  erosion->SetSchedule( pyramid->GetSchedule() );
  erosion->SetIsMovingMask( false );
  erosion->SetResolutionLevel( level );

  /** Set output of the erosion to fixedImageMaskAsImage. */
  FixedMaskImagePointer erodedFixedMaskAsImage = erosion->GetOutput();

  /** Do the erosion. */
  try
  {
    erodedFixedMaskAsImage->Update();
  }
  catch( itk::ExceptionObject & excp )
  {
    /** Add information to the exception. */
    excp.SetLocation( "RegistrationBase - UpdateMasks()" );
    std::string err_str = excp.GetDescription();
    err_str += "\nError while eroding the fixed mask.\n";
    excp.SetDescription( err_str );
    /** Pass the exception to an higher level. */
    throw excp;
  }

  /** Release some memory. */
  erodedFixedMaskAsImage->DisconnectPipeline();

  fixedMaskSpatialObject->SetImage( erodedFixedMaskAsImage );
  return fixedMaskSpatialObject;

} // end GenerateFixedMaskSpatialObject()


/**
 * ******************* GenerateMovingMaskSpatialObject **********************
 */

template <class TElastix>
typename RegistrationBase<TElastix>::MovingMaskSpatialObjectPointer 
RegistrationBase<TElastix>
::GenerateMovingMaskSpatialObject(
  const MovingMaskImageType * maskImage, bool useMaskErosion,
  const MovingImagePyramidType * pyramid, unsigned int level ) const
{
  MovingMaskSpatialObjectPointer movingMaskSpatialObject = 0;
  if ( !maskImage )
  {
    return movingMaskSpatialObject;
  }
  movingMaskSpatialObject = MovingMaskSpatialObjectType::New();

  /** Just convert to spatial object if no erosion is needed. */
  if ( !useMaskErosion || !pyramid )
  {
    movingMaskSpatialObject->SetImage( maskImage );
    return movingMaskSpatialObject;
  }

  /** Erode, and convert to spatial object. */
  MovingMaskErodeFilterPointer erosion = MovingMaskErodeFilterType::New();
  erosion->SetInput( maskImage );
  erosion->SetSchedule( pyramid->GetSchedule() );
  erosion->SetIsMovingMask( true );
  erosion->SetResolutionLevel( level );

  /** Set output of the erosion to movingImageMaskAsImage. */
  MovingMaskImagePointer erodedMovingMaskAsImage = erosion->GetOutput();

  /** Do the erosion. */
  try
  {
    erodedMovingMaskAsImage->Update();
  }
  catch( itk::ExceptionObject & excp )
  {
    /** Add information to the exception. */
    excp.SetLocation( "RegistrationBase - UpdateMasks()" );
    std::string err_str = excp.GetDescription();
    err_str += "\nError while eroding the moving mask.\n";
    excp.SetDescription( err_str );
    /** Pass the exception to an higher level. */
    throw excp;
  }

  /** Release some memory */
  erodedMovingMaskAsImage->DisconnectPipeline();

  movingMaskSpatialObject->SetImage( erodedMovingMaskAsImage );
  return movingMaskSpatialObject;

} // end GenerateMovingMaskSpatialObject()


/**
 * ******************* AfterEachResolutionBase **********************
 */

template <class TElastix>
void
RegistrationBase<TElastix>
::AfterEachResolutionBase( void )
{
  /** What is the current resolution level? */
  const unsigned int level = this->m_Registration->GetAsITKBaseType()->GetCurrentLevel();

  /** Release memory of the previous resolution.
   * This is already done in for example MultiResolutionImageRegistrationMethod2,
   * but perhaps we used another registration method that was not as careful
   * with memory. Therefore, we do it again.
   */
  this->GetElastix()->GetElxFixedImagePyramidBase()->GetAsITKBaseType()
    ->GetOutput( level )->ReleaseData();
  this->GetElastix()->GetElxMovingImagePyramidBase()->GetAsITKBaseType()
    ->GetOutput( level )->ReleaseData();

} // end AfterEachResolutionBase()


} // end namespace elastix

#endif // end #ifndef __elxRegistrationBase_hxx
