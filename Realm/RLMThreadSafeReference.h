////////////////////////////////////////////////////////////////////////////
//
// Copyright 2016 Realm Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
////////////////////////////////////////////////////////////////////////////

#import <Foundation/Foundation.h>

@class RLMRealm;

NS_ASSUME_NONNULL_BEGIN

/**
 An instance which is bound to a thread-specific `RLMRealm` instance, and so cannot be passed
 between threads without being explicitly exported and imported.

 Instances conforming to this protocol can be converted to a thread-safe reference for transport
 between threads by passing to the `+[RLMThreadSafeReference referenceWithThreadConfined:]`
 constructor.

 Note that only types defined by Realm can meaningfully conform to this protocol, and defining new
 classes which attempt to conform to it will not make them work with `RLMThreadSafeReference`.
 */
@protocol RLMThreadConfined <NSObject>
// Conformance to the `RLMThreadConfined_Private` protocol will be enforced at runtime.

/// The Realm which manages the object, or `nil` if the object is unmanaged.
@property (nonatomic, readonly, nullable) RLMRealm *realm;

/// Indicates if the object can no longer be accessed because it is now invalid.
@property (nonatomic, readonly, getter = isInvalidated) BOOL invalidated;

@end

/**
 An object intended to be passed between threads containing a thread-safe reference to its
 thread-confined object.

 To resolve a thread-safe reference on a target Realm on a different thread, pass to
 `-[RLMRealm resolveThreadSafeReference:]`.

 @warning Every `RLMThreadSafeReference` object created must be resolved exactly once.
          An exception will be thrown if a referenced is resolved more than once.

 @note Prefer short-lived `RLMThreadSafeReference`s as the data for the version of the source Realm
       will be retained until all references have been resolved or deallocated.

 @see `RLMThreadConfined`
 @see `-[RLMRealm resolveThreadSafeReference:]`
 */
@interface RLMThreadSafeReference<__covariant Confined : id<RLMThreadConfined>> : NSObject

/**
 Create a thread-safe reference to the thread-confined object.

 @param threadConfined The thread-confined object to create a thread-safe reference to.

 @note You may continue to use and access the thread-confined object after passing it to this
       constructor.
 */
+ (instancetype)referenceWithThreadConfined:(Confined)threadConfined;

/**
 Indicates if the reference can no longer be resolved because the referenced object has been
 deleted.
 */
@property (nonatomic, readonly, getter = isInvalidated) BOOL invalidated;

#pragma mark - Unavailable Methods

/**
 `-[RLMThreadSafeReference init]` is not available because `RLMThreadSafeReference` cannot be
 created directly. `RLMThreadSafeReference` instances must be obtained by calling
 `-[RLMRealm resolveThreadSafeReference:]`.
 */
- (instancetype)init __attribute__((unavailable("RLMThreadSafeReference cannot be created directly")));

/**
 `-[RLMThreadSafeReference new]` is not available because `RLMThreadSafeReference` cannot be
 created directly. `RLMThreadSafeReference` instances must be obtained by calling
 `-[RLMRealm resolveThreadSafeReference:]`.
 */
+ (instancetype)new __attribute__((unavailable("RLMThreadSafeReference cannot be created directly")));

@end

NS_ASSUME_NONNULL_END
